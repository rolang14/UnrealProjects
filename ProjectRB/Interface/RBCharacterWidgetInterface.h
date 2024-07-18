// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RBCharacterWidgetInterface.generated.h"


struct FInputActionValue;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URBCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTRB_API IRBCharacterWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupInteractWidget(class URBInteractWidget*) = 0;
	virtual void InputActionChangeInteract(const FInputActionValue& Value) = 0;
};
