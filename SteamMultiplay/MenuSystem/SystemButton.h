// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SystemButton.generated.h"


class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API USystemButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UButton* ButtonHitArea;
	
private:
	UPROPERTY(Meta = (BindWidget))
	UImage* ButtonBackground;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ButtonTextBlock;
};
