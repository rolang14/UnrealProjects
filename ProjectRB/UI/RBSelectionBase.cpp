// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RBSelectionBase.h"
#include "CommonTextBlock.h"


URBSelectionBase::URBSelectionBase()
{
	CurrentSelection = 0;
	bIsFocusable = true;
	// 생성자 내부에서 Visibility 설정 방법
	SetVisibilityInternal(ESlateVisibility::Visible);
}

void URBSelectionBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (Options.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("URBSelectionBase : No Options where provided."));
		return;
	}

	UpdateCurrentSelection();
}

void URBSelectionBase::Clear()
{
}

void URBSelectionBase::AddOption(const FRBSelectionOption& InOption)
{
}

void URBSelectionBase::SetCurrentSelection(int InIndex)
{
}

void URBSelectionBase::SelectPrevious()
{
}

void URBSelectionBase::SelectNext()
{
}

UWidget* URBSelectionBase::OnNavigation(EUINavigation InNavigation)
{
	return nullptr;
}

void URBSelectionBase::UpdateCurrentSelection()
{
	Label->SetText(Options[CurrentSelection].Label);
}
