// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/TTBasePawn.h"
#include "InputActionValue.h"
#include "TTTankPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANK_API ATTTankPawn : public ATTBasePawn
{
	GENERATED_BODY()

public:
	ATTTankPawn();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void HandleDestruction() override; 
	bool IsPlayerDead = false;

	// class ATTPlayerController* GetPlayerController() const { return PlayerController; }

protected:
	virtual void BeginPlay() override;
	
private:
	void InputActionMove(const FInputActionValue& Value);
	void InputActionRotate(const FInputActionValue& Value);
	void InputActionFire();

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> RotateAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UCameraComponent> Camera;

	// TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> PlayerInputSubsystem;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotateSpeed = 150.0f;

private:
	class ATTPlayerController* PlayerController;
};
