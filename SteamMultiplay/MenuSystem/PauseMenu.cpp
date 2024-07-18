// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "MenuSystem/SystemButton.h"
#include "MenuSystem/MenuInterface.h"

#include "Components/Button.h"


bool UPauseMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!WBP_SystemButton_Leave || !WBP_SystemButton_Cancel) return false;
	WBP_SystemButton_Leave->ButtonHitArea->OnClicked.AddDynamic(this, &UPauseMenu::LeaveGame);
	WBP_SystemButton_Cancel->ButtonHitArea->OnClicked.AddDynamic(this, &UPauseMenu::CloseMenu);

	return true;
}

void UPauseMenu::NativeDestruct()
{
	UE_LOG(LogTemp, Display, TEXT("Remove From Viewport 하면 소멸자가 호출될까?"));
}

void UPauseMenu::LeaveGame()
{
	if (MenuInterface)
	{
		ClearWidget();
		MenuInterface->LeaveGame();
	}
}

void UPauseMenu::CloseMenu()
{
	ClearWidget();
}
