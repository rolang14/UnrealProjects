// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBCharacterPlayer.h"
#include "Controller/RBPlayerController.h"

#include "InputActionValue.h"

#include "Data/RBCharacterControlData.h"
#include "Interact/RBCharacterInteractComponent.h"
#include "UI/RBInteractWidget.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"


ARBCharacterPlayer::ARBCharacterPlayer()
{
	// 스프링 암 및 카메라 컴포넌트 생성
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Character Camera Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	CharacterInteractComponent = CreateDefaultSubobject<URBCharacterInteractComponent>(TEXT("Interact Manager"));
}

// RBPlayerController 에 의해서 Possess 되었다면 델리게이트를 구독한다.
void ARBCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 필요한 델리게이트를 여기서 바인딩한다.
	ARBPlayerController* PlayerController = Cast<ARBPlayerController>(NewController);
	if (PlayerController)
	{
		// For Unpossessed Unbind
		ExPlayerController = PlayerController;

		// For Setup PlayerMode Delegate binding
		PlayerController->PlayerModeChangedDelegate.BindUObject(this, &ARBCharacterPlayer::ChangeCharacterMovementMode);
		PlayerController->SetCurrentPlayerMode(PlayerController->GetCurrentPlayerMode());

		// For Setup Interact Delegate binding
		CharacterInteractComponent->OnInteractChanged.AddUObject(PlayerController, &ARBPlayerController::ChangeWheelMode);
	}
}

// 컨트롤러 델리게이트 해제용
void ARBCharacterPlayer::UnPossessed()
{
	Super::UnPossessed();

	if (ExPlayerController)
	{
		ExPlayerController->PlayerModeChangedDelegate.Unbind();
		ExPlayerController = nullptr;
	}
}

void ARBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 캐릭터의 각종 컨트롤 데이터 세팅
	//SetCharacterControlData();

	NextZoom = SpringArmComponent->TargetArmLength;
}

void ARBCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Smooth Zoom 수행
	if (SpringArmComponent->TargetArmLength != NextZoom)
	{
		PerformSmoothZoom();
	}
}

#pragma region Input Binding Functions

// 카메라 회전
void ARBCharacterPlayer::InputActionLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	// 마우스 입력은 프레임레이트와 관계 없이 해당 프레임에서의 가속도가 들어오기 때문에 DT 고려하지 않아도 된다. 
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

// 캐릭터 움직임
void ARBCharacterPlayer::InputActionMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	ProcessCharacterMove(MovementVector.Y, MovementVector.X);
}

// 카메라 -> 캐릭터 줌 인/아웃
void ARBCharacterPlayer::InputActionZoom(const FInputActionValue& Value)
{
	SetNextZoom(Value.Get<float>());
}

// 캐릭터 점프
void ARBCharacterPlayer::InputActionJump()
{
	// 필드 공격 수행 중에는 점프를 할 수 없음.
	if (!IsProcessingFieldAttack())
	{
		UE_LOG(LogTemp, Display, TEXT("Jump"));
		Jump();
	}
}

// 캐릭터 점프 종료
void ARBCharacterPlayer::InputActionStopJumping()
{
	StopJumping();
}

// 캐릭터 걷기/뛰기 모드 변경
void ARBCharacterPlayer::InputActionChangeRunMode()
{
	if (IsRunMode)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentCharacterControlData->MinWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentCharacterControlData->MaxWalkSpeed;
	}

	IsRunMode = !IsRunMode;
}

// 캐릭터의 필드 공격 (전투 진입 공격)
void ARBCharacterPlayer::InputActionFieldAttack()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		// 공중에서는 필드 공격 수행 불가
		if (!Movement->IsFalling())
		{
			ProcessFieldAttack();
		}
	}
}

// 선택할 상호작용 변경
void ARBCharacterPlayer::InputActionChangeInteract(const FInputActionValue& Value)
{
	// 마우스 휠 움직임을 받아서 위쪽은 위에 있는 것으로, 아래쪽은 아래에 있는 것으로 SelectedIndex 를 변경.
	// 위쪽이 -1, 아래쪽은 1
	if (CharacterInteractComponent)
	{
		CharacterInteractComponent->ChangeInteractIndex(FMath::FloorToInt32(Value.Get<float>()));
	}
}

#pragma endregion

#pragma region Character Control Functions

// 상세 캐릭터 컨트롤 설정
void ARBCharacterPlayer::SetCharacterControlData()
{
	if (CurrentCharacterControlData)
	{
		// Spring Arm이 Pawn의 컨트롤을 따라 회전한다. (False 인 경우 카메라는 Pawn의 위치를 고수하며, 컨트롤에 따라 회전하지 않음)
		SpringArmComponent->bUsePawnControlRotation = CurrentCharacterControlData->bUsePawnControlRotation;
		SpringArmComponent->bInheritPitch = CurrentCharacterControlData->bInheritPitch;
		SpringArmComponent->bInheritYaw = CurrentCharacterControlData->bInheritYaw;
		SpringArmComponent->bInheritRoll = CurrentCharacterControlData->bInheritRoll;
		SpringArmComponent->TargetArmLength = CurrentCharacterControlData->TargetArmLength;

		// 캐릭터 키에 맞춰 스프링암 높이 세팅
		SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, CameraHeight));
		SpringArmComponent->SocketOffset = CurrentCharacterControlData->SpringArmSocketOffset;

		// Pawn의 Yaw 회전이 Player Controller 의 Yaw 회전을 따라가게 한다.
		bUseControllerRotationYaw = CurrentCharacterControlData->bUseControllerRotationYaw;

		// bOrientRotationToMovement = 캐릭터를 가속도 방향으로 회전시킬 것인지에 대한 옵션. 추가로 RotationRate 를 사용하여 회전 속도를 정한다.
		GetCharacterMovement()->bOrientRotationToMovement = CurrentCharacterControlData->bOrientRotationToMovement;
		GetCharacterMovement()->bUseControllerDesiredRotation = CurrentCharacterControlData->bUseControllerDesiredRotation;
		GetCharacterMovement()->RotationRate = CurrentCharacterControlData->RotationRate;
		GetCharacterMovement()->MaxWalkSpeed = CurrentCharacterControlData->MaxWalkSpeed;
	}
}

// 현재 플레이어 상태에 따른 움직임 제한
void ARBCharacterPlayer::ChangeCharacterMovementMode(const EPlayerMode& PlayerMode)
{
	switch (PlayerMode)
	{
	case EPlayerMode::FIELD_NORMAL :
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		CurrentCharacterControlData = OnFieldCharacterControlData;
		GetMesh()->SetAnimInstanceClass(OnFieldAnimationBP);
		break;
	case EPlayerMode::FIELD_INTERACTING :
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		CurrentCharacterControlData = OnFieldCharacterControlData;
		break;
	case EPlayerMode::BATTLE_NORMAL:
		CurrentCharacterControlData = InBattleCharacterControlData;
		GetMesh()->SetAnimInstanceClass(InBattleAnimationBP);
		break;
	}

	SetCharacterControlData();
}

void ARBCharacterPlayer::ProcessCharacterMove(const float& MoveForwardValue, const float& MoveRightValue)
{
	// 필드 공격 모션 캔슬
	if (CanInterruptFieldAttack() && IsProcessingFieldAttack())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.1f, FieldAttackMontage);
		}
	}

	// 캐릭터가 카메라는 고정된 채로, WASD 컨트롤에 따라 회전하며 움직이게 하고 싶으므로
	// 아래와 같이 액터의 방향으로 시도하면 회전하면서 계속 Right Vector 가 변경되므로
	// 컨트롤러의 Yaw 방향을 가져와야 한다.
	//AddMovementInput(GetActorForwardVector() * MovementVector.X);
	//AddMovementInput(GetActorRightVector() * MovementVector.Y);

	// 컨트롤러의 Rotation 을 가져온다.
	const FRotator ControllerYawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ControllerForwardDirection = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);
	const FVector ControllerRightDirection = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ControllerForwardDirection, MoveForwardValue);
	AddMovementInput(ControllerRightDirection, MoveRightValue);
}

void ARBCharacterPlayer::SetNextZoom(const float ZoomValue)
{
	const float NextZoomValue = SpringArmComponent->TargetArmLength + ZoomValue * CurrentCharacterControlData->ZoomSpeed;
	NextZoom = FMath::Clamp(NextZoomValue, TargetArmMinLength, TargetArmMaxLength);
}

void ARBCharacterPlayer::PerformSmoothZoom()
{
	// FInterpTo 이나 EaseOut 혹은 Lerp 을 사용해도 좋다. 한번 비교해보자.
	switch (CurrentCharacterControlData->SmoothType)
	{
	case SmoothType::Default:
		SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, NextZoom, GetWorld()->GetDeltaSeconds(), CurrentCharacterControlData->SmoothZoomSpeed);
		break;
	case SmoothType::EaseOut:
		SpringArmComponent->TargetArmLength = FMath::InterpEaseOut(SpringArmComponent->TargetArmLength, NextZoom, GetWorld()->GetDeltaSeconds(), CurrentCharacterControlData->SmoothZoomSpeed);
		break;
	case SmoothType::Lerp:
		SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, NextZoom, GetWorld()->GetDeltaSeconds() * CurrentCharacterControlData->SmoothZoomSpeed);
		break;
	}
}

#pragma endregion

#pragma region Play Animation

void ARBCharacterPlayer::ProcessFieldAttack()
{
	if (IsProcessingFieldAttack())
	{
		return;
	}
	else
	{
		FieldAttack();
	}
}

#pragma endregion

// 위젯 셋업
void ARBCharacterPlayer::SetupInteractWidget(URBInteractWidget* RBInteractWidget)
{
	if (CharacterInteractComponent)
	{
		// Interact Component 을 InteractWidget 이 반영할 수 있도록 델리게이트 바인딩
		CharacterInteractComponent->OnInteractChanged.AddUObject(RBInteractWidget, &URBInteractWidget::UpdateInteractButton);
		CharacterInteractComponent->OnSelectedInteractChanged.AddUObject(RBInteractWidget, &URBInteractWidget::UpdateSelectedInteract);
	}
}