// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/MenuWidgetBase.h"
#include "PauseMenu.generated.h"


class USystemButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPauseMenu : public UMenuWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	// 현재 게임에서 나가고 메인 메뉴로 돌아간다.
	UFUNCTION()
	void LeaveGame();

	// 메뉴 창을 종료하고 게임으로 돌아간다.
	UFUNCTION()
	void CloseMenu();
	
private:
	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_Leave;

	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_Cancel;
};
