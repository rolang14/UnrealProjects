// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSCharacterMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USSCharacterMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLESHOOTER_API ISSCharacterMovementInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Move(const struct FInputActionValue& InputValue) = 0;
	virtual void Look(const struct FInputActionValue& InputValue) = 0;
	virtual void LookController(const struct FInputActionValue& InputValue) = 0;
	virtual void Jump() = 0;
	virtual void StopJumping() = 0;
};
