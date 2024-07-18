// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Game")
	float GameRestartTime = 5.f;

// Input Mapping Section
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> LookControllerAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> ReloadAction;

// UI Section
private:
	UPROPERTY(EditAnywhere, Category = "Player UI")
	TSubclassOf<class UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> CrosshairWidget;

	UPROPERTY(EditAnywhere, Category = "Screen UI")
	TSubclassOf<class UUserWidget> UserWinWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Screen UI")
	TSubclassOf<class UUserWidget> UserLoseWidgetClass;

private:
	FTimerHandle GameRestartTimerHandle;

	void Move(const struct FInputActionValue& InputValue);
	void Look(const struct FInputActionValue& InputValue);
	void LookController(const struct FInputActionValue& InputValue);
	void Jump();
	void StopJumping();

	void Fire();
	void Reload();
};
