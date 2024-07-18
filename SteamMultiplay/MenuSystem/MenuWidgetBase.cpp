// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MenuWidgetBase.h"

void UMenuWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UMenuWidgetBase::Initialize()
{
	if (!Super::Initialize()) return false;
	return true;
}

// UI 설정 및 플레이어 입력 처리
void UMenuWidgetBase::SetUp()
{
	UE_LOG(LogTemp, Display, TEXT("Set Up !!"));

	SetIsFocusable(true);

	this->AddToViewport();

	// 우선 GetOwningPlayer 를 통해 플레이어 컨트롤러를 얻어 오는데,
	// 혹시나 멀티플레이어 등 나중에 문제가 생긴다면 World를 얻어와서 World에서 플레이어를 얻도록 하자.
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr) return;

	PlayerController->bShowMouseCursor = true;

	FInputModeUIOnly MainMenuInputMode;
	MainMenuInputMode.SetWidgetToFocus(this->TakeWidget());
	MainMenuInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(MainMenuInputMode);
}

void UMenuWidgetBase::ClearWidget()
{
	this->RemoveFromViewport();
	SetIsFocusable(false);

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr) return;

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
}
