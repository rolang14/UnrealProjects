// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SSPlayerController.h"
#include "Interface/SSCharacterMovementInterface.h"
#include "Interface/SSCharacterAttackInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"


void ASSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CrosshairWidget = CreateWidget(this, CrosshairWidgetClass);
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
}

// OnPossess 시에 폰에게 인풋 매핑 시도
void ASSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedSubsystem->ClearAllMappings();
		EnhancedSubsystem->AddMappingContext(InputMappingContext, 0);
	}
}

// Enhanced Input Action 바인딩
void ASSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASSPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASSPlayerController::Look);
		EnhancedInputComponent->BindAction(LookControllerAction, ETriggerEvent::Triggered, this, &ASSPlayerController::LookController);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASSPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASSPlayerController::StopJumping);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ASSPlayerController::Fire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ASSPlayerController::Reload);
	}
}

#pragma region Input Binding Functions

void ASSPlayerController::Move(const FInputActionValue& InputValue)
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterMovementInterface* MovePawn = Cast<ISSCharacterMovementInterface>(PosseingPawn))
	{
		MovePawn->Move(InputValue);
	}
}

void ASSPlayerController::Look(const FInputActionValue& InputValue)
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterMovementInterface* MovePawn = Cast<ISSCharacterMovementInterface>(PosseingPawn))
	{
		MovePawn->Look(InputValue);
	}
}

void ASSPlayerController::LookController(const FInputActionValue& InputValue)
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterMovementInterface* MovePawn = Cast<ISSCharacterMovementInterface>(PosseingPawn))
	{
		MovePawn->LookController(InputValue);
	}
}

void ASSPlayerController::Jump()
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterMovementInterface* MovePawn = Cast<ISSCharacterMovementInterface>(PosseingPawn))
	{
		MovePawn->Jump();
	}
}

void ASSPlayerController::StopJumping()
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterMovementInterface* MovePawn = Cast<ISSCharacterMovementInterface>(PosseingPawn))
	{
		MovePawn->StopJumping();
	}
}

void ASSPlayerController::Fire()
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterAttackInterface* MovePawn = Cast<ISSCharacterAttackInterface>(PosseingPawn))
	{
		MovePawn->Fire();
	}
}

void ASSPlayerController::Reload()
{
	APawn* PosseingPawn = GetPawn();
	if (ISSCharacterAttackInterface* MovePawn = Cast<ISSCharacterAttackInterface>(PosseingPawn))
	{
		MovePawn->Reload();
	}
}

#pragma endregion


// 게임 종료 시
void ASSPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (CrosshairWidget)
	{
		CrosshairWidget->RemoveFromViewport();
	}

	// 결과 UI를 Viewport에 표시
	UUserWidget* const ResultScreen = bIsWinner ? CreateWidget(this, UserWinWidgetClass) : CreateWidget(this, UserLoseWidgetClass);
	if (ResultScreen)
	{
		ResultScreen->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(GameRestartTimerHandle, this, &APlayerController::RestartLevel, GameRestartTime, false);
}