// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/ServerItem.h"
#include "MenuSystem/MainMenu.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"


void UServerItem::Setup(UMainMenu* InParent, const uint32 InIndex)
{
	Index = InIndex;
	Parent = InParent;

	Button_ServerItem->OnClicked.AddDynamic(this, &UServerItem::OnItemClicked);
}

void UServerItem::SetServerNameText(const FText InServerName)
{
	TextBlock_ServerName->SetText(InServerName);
}

void UServerItem::SetHostNameText(const FText InHostName)
{
	TextBlock_ServerHostName->SetText(InHostName);
}

void UServerItem::SetPlayerCountText(const FText InPlayerCount)
{
	TextBlock_ServerPlayerCount->SetText(InPlayerCount);
}

void UServerItem::OnItemClicked()
{
	Parent->SetSelectedIndex(Index);
}
