// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PuzzlePlayerController.generated.h"


class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API APuzzlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetupEnhancedInput();

protected:
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> LoadMenuAction;
};
