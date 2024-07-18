// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RBPlayerController.h"
#include "GameMode/RBOnFieldGameMode.h"

#include "Data/RBInteractData.h"
#include "Interact/RBCharacterInteractComponent.h"
#include "Interface/RBCharacterMovementInterface.h"
#include "Interface/RBCharacterWidgetInterface.h"
#include "Interface/RBInteractInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ARBPlayerController::ARBPlayerController()
{
	WheelMode = EMouseWheelMode::ZOOM;

	// Wheel Mode에 따라 다른 Wheel 동작 구현
	WheelModeChangedActions.Emplace(EMouseWheelMode::ZOOM, FWheelModeChangedDelegateWrapper(FOnWheelModeChangedDelegate::CreateUObject(this, &ARBPlayerController::Zoom)));
	WheelModeChangedActions.Emplace(EMouseWheelMode::INTERACT, FWheelModeChangedDelegateWrapper(FOnWheelModeChangedDelegate::CreateUObject(this, &ARBPlayerController::ChangeInteract)));

	// Player Mode에 따라 다른 Interact 동작 구현
	PlayerModeChangedActions.Emplace(EPlayerMode::FIELD_NORMAL, FPlayerModeChangedDelegateWrapper(FOnPlayerModeChangedDelegate::CreateUObject(this, &ARBPlayerController::RunInteract)));
	PlayerModeChangedActions.Emplace(EPlayerMode::FIELD_INTERACTING, FPlayerModeChangedDelegateWrapper(FOnPlayerModeChangedDelegate::CreateUObject(this, &ARBPlayerController::ProcessDialogue)));
}

void ARBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 이후의 파티 멤버 추가를 어떻게 구현할 것인지는 좀 생각해 봐야 할 듯.
	// 지금으로써는 다이얼로그 대화 이후 파티에 합류하는 것까지만 생각하자.
	// 다이얼로그 시스템 구현 이후 여기로 돌아올 것 

	// UI 세팅
	OnFieldHUD = CreateWidget(this, OnFieldHUDClass);
	if (OnFieldHUD)
	{
		OnFieldHUD->AddToViewport();
	}
}

void ARBPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Set Enhanced Input Context
	SetCharacterEnhancedInput();
}

// Enhanced Input Subsystem Setting
void ARBPlayerController::SetCharacterEnhancedInput()
{
	check(InputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);
	// PlayerController->SetPlayerControllerEnabled(true);
}

// Enhanced Input Binding
void ARBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Move);
	EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Wheel);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARBPlayerController::StopJumping);
	EnhancedInputComponent->BindAction(ChangeRunModeAction, ETriggerEvent::Triggered, this, &ARBPlayerController::ChangeRunMode);
	EnhancedInputComponent->BindAction(FieldAttackAction, ETriggerEvent::Triggered, this, &ARBPlayerController::FieldAttack);
	EnhancedInputComponent->BindAction(ChangeCharacterAction, ETriggerEvent::Triggered, this, &ARBPlayerController::SwitchActiveCharacter);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Interact);
}

#pragma region Input Actions

void ARBPlayerController::Look(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionLook(Value);
	}
}

void ARBPlayerController::Move(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionMove(Value);
	}
}

void ARBPlayerController::Wheel(const FInputActionValue& Value)
{
	// 현재 휠 모드에 맞는 액션 수행
	WheelModeChangedActions[WheelMode].WheelModeChangedDelegate.ExecuteIfBound(Value);
}

void ARBPlayerController::Interact()
{
	PlayerModeChangedActions[GetCurrentPlayerMode()].PlayerModeChangedDelegate.ExecuteIfBound();
}

void ARBPlayerController::Jump()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionJump();
	}
}

void ARBPlayerController::StopJumping()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionStopJumping();
	}
}

void ARBPlayerController::ChangeRunMode()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionChangeRunMode();
	}
}

void ARBPlayerController::FieldAttack()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionFieldAttack();
	}
}

// 메인 캐릭터 변경
void ARBPlayerController::SwitchActiveCharacter()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		ARBOnFieldGameMode* RBOnFieldGameMode = Cast<ARBOnFieldGameMode>(UGameplayStatics::GetGameMode(this));
		if (RBOnFieldGameMode == nullptr)
		{
			return;
		}

		// 눌린 키를 확인 후, N번 캐릭터와 플레이를 교체한다.
		const TArray<FKey> ChangeKeys = Subsystem->QueryKeysMappedToAction(ChangeCharacterAction);
		for (const FKey Key : ChangeKeys)
		{
			if (IsInputKeyDown(Key))
			{
				UE_LOG(LogTemp, Display, TEXT("Character Switch to : %s"), *Key.GetDisplayName().ToString());
				// Get GameMode
				// Cast To My GameMode
				// Call Function 'Swap to Chacter' or sth From GameMode
				RBOnFieldGameMode->SwitchPlayerCharacter(this, FCString::Atoi(*Key.GetDisplayName().ToString()));
			}
		}
	}
}

// Wheel - Zoom Action
void ARBPlayerController::Zoom(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionZoom(Value);
	}
}

// Wheel - Interact Change Action
void ARBPlayerController::ChangeInteract(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterWidgetInterface* PawnWidgetAction = Cast<IRBCharacterWidgetInterface>(PossessingPawn))
	{
		PawnWidgetAction->InputActionChangeInteract(Value);
	}
}

// 상호작용 실행 키
void ARBPlayerController::RunInteract()
{
	// Interface Implement
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	// 이게 맞는 방법인지는 잘 모르겠다..
	if (IRBInteractInterface* InteractInterface = Cast<IRBInteractInterface>(PossessingPawn->GetComponentByClass(URBCharacterInteractComponent::StaticClass())))
	{
		InteractInterface->RunInteract(this);
	}
}

// 다이얼로그 넘기기 키
void ARBPlayerController::ProcessDialogue()
{
	// Interface Implement
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	// 이게 맞는 방법인지는 잘 모르겠다..
	if (IRBDialogueInterface* DialogueInterface = Cast<IRBDialogueInterface>(PossessingPawn->GetComponentByClass(URBCharacterInteractComponent::StaticClass())))
	{
		DialogueInterface->ProcessDialogue();
	}
}

#pragma endregion

// 휠 모드 변경 (일반 : 줌인/아웃, 상호작용이 2개 이상 : 상호작용 변경)
void ARBPlayerController::ChangeWheelMode(const TArray<const URBInteractData*>& InteractDataList)
{
	WheelMode = InteractDataList.Num() > 1 ? EMouseWheelMode::INTERACT : EMouseWheelMode::ZOOM;
}