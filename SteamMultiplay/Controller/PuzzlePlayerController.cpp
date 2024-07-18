// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PuzzlePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void APuzzlePlayerController::SetupEnhancedInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APuzzlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Look);
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Move);
		//EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Wheel);

		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARBPlayerController::StopJumping);

		//EnhancedInputComponent->BindAction(ChangeRunModeAction, ETriggerEvent::Triggered, this, &ARBPlayerController::ChangeRunMode);

		//EnhancedInputComponent->BindAction(FieldAttackAction, ETriggerEvent::Triggered, this, &ARBPlayerController::FieldAttack);

		//EnhancedInputComponent->BindAction(ChangeCharacterAction, ETriggerEvent::Triggered, this, &ARBPlayerController::SwitchActiveCharacter);

		//EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Interact);
	}
}
