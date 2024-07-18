// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBFieldToBattleTransitionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRB_API URBFieldToBattleTransitionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayTransition();
	
public:
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> TransitionAnim;
};
