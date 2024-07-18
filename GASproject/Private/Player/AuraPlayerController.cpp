// Copyright NHE


#include "Player/AuraPlayerController.h"
#include "Interaction/HighlightInterface.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "UI/Widget/DamageTextComponent.h"

#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"


AAuraPlayerController::AAuraPlayerController()
{
	// Enable Server-Client Replicate
	bReplicates = true;

	AutoRunningSpline = CreateDefaultSubobject<USplineComponent>("AutoRunSpline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupInputSystem();
}

void AAuraPlayerController::SetupInputSystem()
{
	check(InputMappingContext);
	// UEnhancedInputLocalPlayerSubsystem 은 싱글톤이다.
	// 멀티플레이 환경의 경우, Subsystem 은 LocalPlayer 가 없는 경우에 nullptr 이 된다. 즉, 이때도 Assert 보다는 if를 활용해서 Subsystem 이 존재하는 경우에만 수행하면 된다.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeGameAndUI);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHold);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	bIsAutoRunning = false;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	// 컨트롤러의 방향 취득. 컨트롤러의 방향은 카메라->캐릭터의 방향이다.
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	// Forward Vector 계산을 위해 RotationMatrix에서 원하는 UnitAxis를 가져온다.
	// UnitAxis 이므로 언제나 Normalized 된 벡터 (1) 을 반환한다.
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 오른쪽 벡터 : Y
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 여기서 Check를 강하게 하지 않는 이유는
	// Move의 경우 우리가 키를 계속해서 누르고 있을 것이고, 이는 매 프레임마다 실행이 되기 때문에
	// 잠재적으로 Pawn을 가져오지 못할 수도 있고, 또 가져오지 못한다고 하더라도 다음 프레임에 계산을 하면 되니까
	// 사실상 아무런 문제가 없다. 그렇기 때문에 If statement 로 처리한다.
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorChase();

	// For Auto Running
	PerformAutoRun();
}

// 마우스 커서를 통해 Trace하고, 특정 액터를 Highlight 한다.
void AAuraPlayerController::CursorChase()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastHighlightedActor = CurHighlightActor;
	// TScriptInterface 래퍼를 사용하면 굳이 인터페이스 캐스팅을 안해도 된다. 알아서 해준다.
	// 아무것도 없으면 알아서 nullptr 이 된다.
	CurHighlightActor = CursorHit.GetActor();

	// 만약 이전거랑 지금거랑 동일하면, 어차피 이전것이 Highlight 되어있을거니까 아무것도 안해도 된다. null 인 경우에도 null이니까 아무것도 안해도 된다. (최적화)
	if (LastHighlightedActor.GetObject() != CurHighlightActor.GetObject())
	{
		if (LastHighlightedActor.GetInterface()) LastHighlightedActor->UnHighlightActor();
		if (CurHighlightActor.GetInterface()) CurHighlightActor->HighlightActor();
	}
}

void AAuraPlayerController::PerformAutoRun()
{
	if (!bIsAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		// Spline 을 따라 움직여야 하는데, 아래는 Spline 과 Pawn 사이의 최단거리 포인트를 반환한다. (굳이 이렇게 안해도 그냥 GetActorLocation() 로도 충분하긴 하다)
		const FVector LocationOnSpline = AutoRunningSpline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::Type::World);
		// Spline 을 따라 움직여야 할 방향
		const FVector DirectionToSpline = AutoRunningSpline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::Type::World);
		ControlledPawn->AddMovementInput(DirectionToSpline);

		// 도착했는지?
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination < AutoRunCompleteRadius)
		{
			bIsAutoRunning = false;
		}
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(const float DamageAmount, ACharacter* Target)
{
	// IsValid : Nullptr 체크와 동시에 Pending Kill 상태인지 체크함. 현재 캐릭터의 위치를 알고 싶기 때문에 Pending Kill 상태면 곤란하다.
	// 때문에 캐릭터나 삭제될 수 있는 오브젝트를 참조해야 하는 경우에는 웬만하면 IsValid 로 체크하는 것이 좋다.
	// DamageTextComponent 에 IsValid 를 사용하지 않은 이유는, Subclass 이기 때문에 단순히 설정이 되었는지 아닌지만 확인하면 되기 때문이다. 물론 반드시 설정되어있음을 전제로 하므로 check() 를 사용해서 Assert 체크를 해도 된다.
	if (IsValid(Target) && DamageTextComponent)
	{
		// Component 를 생성자가 아니라 Dynamic 하게 만드려면 아래의 절차가 필요하다. 또, WBP의 Constructor 에 애니메이션 재생을 넣어 놓았기 때문에 생성 후에 자동으로 재생이 된다.
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(Target, DamageTextComponent);
		DamageText->RegisterComponent();
		// 처음엔 타겟의 머리 위에 표시되었다가, 적이 움직여도 그냥 그 자리에서 떠다니게 하고 싶다.
		// 따라서 처음 위치 설정 이후에는 Detach 되어야 한다!
		DamageText->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// 마우스 왼쪽 클릭을 이동으로 고정할 것이기 때문에 우선은 이렇게 LMB 태그를 밖자.
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bIsAutoRunning = false;
		// Highlight 중인 액터가 있으면 현재 타게팅이 되어있는 것이고, 아니면 그 방향으로 이동하면 된다.
		bIsTargeting = CurHighlightActor ? true : false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// Hold 의 경우 누르고 있는 프레임마다 호출이 되기 때문에 AuraASC로 캐스팅할 경우 너무 비용이 크다. 그래서 Getter 를 사용한다.
		if (GetAuraASC()) GetAuraASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	GetAuraASC()->AbilityInputTagReleased(InputTag);

	if (!bIsTargeting && !bIsShiftPressed)
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn && FollowTime <= ShortPressThreshold)
		{
			// Create Navigation Path for Auto Running
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				// NavPath 의 Point 를 이용해서 Spline 에 포인트를 추가하여 만든다.
				AutoRunningSpline->ClearSplinePoints();
				for (auto& SplinePoint : NavPath->PathPoints)
				{
					AutoRunningSpline->AddSplinePoint(SplinePoint, ESplineCoordinateSpace::Type::World);
				}
				// 오브젝트 등으로 가려져 있어 도달할 수 없는 위치로 이동하려는 경우를 위해 자동으로 찾은 경로의 마지막 포인트로 목적지를 설정한다.
				// 근데 이렇게 하면 정확하게 목적지로 가는게 아니라 그 전 지점으로 가는거라 좀 부정확하지 않나..?
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bIsAutoRunning = true;
				}
			}
		}

		bIsTargeting = false;
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHold(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// Hold 의 경우 누르고 있는 프레임마다 호출이 되기 때문에 AuraASC로 캐스팅할 경우 너무 비용이 크다. 그래서 Getter 를 사용한다.
		if (GetAuraASC()) GetAuraASC()->AbilityInputTagHold(InputTag);
	}
	else
	{
		if (bIsTargeting || bIsShiftPressed) GetAuraASC()->AbilityInputTagHold(InputTag);
		else if (APawn* ControlledPawn = GetPawn())
		{
			// Do Move
			FollowTime += GetWorld()->GetDeltaSeconds();
			// 이동할 위치 저장. 어차피 매 Tick 마다 커서 위치를 Highlight 에서 Trace 하므로. Hold 되었을 때 이 위치를 사용하면 된다.
			if (CursorHit.bBlockingHit)	CachedDestination = CursorHit.ImpactPoint;

			// 움직일 방향을 구해서 Input.
			FVector MoveDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(MoveDirection);
		}
	}
}

// 만약 Tick이나 Hold Functoin 처럼 Cast를 프레임마다 계속 해야해서 코스트 문제가 있다면, 아래처럼 Getter 를 만들어서 사용하면 된다.
UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
	if (AuraASC == nullptr) AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	return AuraASC;
}