// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RBInteractWidget.h"
#include "UI/RBInteractButtonWidget.h"

#include "Data/RBInteractData.h"

#include "Components/GridSlot.h"
#include "Components/GridPanel.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"


void URBInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Display, TEXT("[Interact UI] Constructor."));

	InteractGridPanel = Cast<UGridPanel>(GetWidgetFromName(TEXT("InteractGridPanel")));
	ensure(InteractGridPanel);

	InteractKeyOverlay = Cast<UOverlay>(GetWidgetFromName(TEXT("InteractKeyOverlay")));
	ensure(InteractKeyOverlay);

	InteractKeyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_InteractKey")));
	ensure(InteractKeyText);

	SetKeyOverlayAvailable(false);
}

// 상호작용 버튼 업데이트 함수
void URBInteractWidget::UpdateInteractButton(const TArray<const URBInteractData*>& InteractDataList)
{
	// Button 과 Data 를 앞에서부터 비교하여, 동일하지 않으면 삭제하는 방식으로 순서를 맞춤.
	int32 idx = 0;
	while (idx < InteractButtons.Num())
	{
		if (idx >= InteractDataList.Num() || !InteractDataList[idx]->DisplayMessage.EqualTo(InteractButtons[idx]->GetDisplayText()))
		{
			RemoveInteractButton(idx--);
		}
		idx++;
	}

	while (idx < InteractDataList.Num())
	{
		CreateInteractButton(InteractDataList[idx]->DisplayMessage, idx);
		idx++;
	}

	SetKeyOverlayAvailable(InteractDataList.Num() > 0);
}

// 상호작용 버튼 위젯 생성
void URBInteractWidget::CreateInteractButton(const FText DisplayMessage, const int32 Index)
{
	URBInteractButtonWidget* InteractButton = CreateWidget<URBInteractButtonWidget>(this, InteractButtonWidgetClass);
	if (InteractButton == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("[Interact UI] Something wrong with creating Interact Button."));
		return;
	}

	InteractGridPanel->AddChildToGrid(InteractButton, Index, 1);
	InteractButton->SetDisplayText(DisplayMessage);
	InteractButtons.Add(InteractButton);
}

// Index 위치의 상호작용 버튼 삭제
void URBInteractWidget::RemoveInteractButton(const int32 Index)
{
	// 해당 위치 버튼 삭제 요청 및 배열에서 삭제
	InteractButtons[Index]->DestructInteract();
	InteractButtons.RemoveAt(Index);

	// 남아있는 버튼이 있다면, Index 뒤의 원소들을 하나씩 땡겨 준다.
	for (int idx = Index; idx < InteractButtons.Num(); idx++)
	{
		UGridSlot* GridSlot = Cast<UGridSlot>(InteractButtons[idx]->Slot);
		ensure(GridSlot);

		int32 RowPull = GridSlot->GetRow() - 1;
		GridSlot->SetRow(RowPull);
	}
}

// 선택된 상호작용 표시 위치 변경
void URBInteractWidget::UpdateSelectedInteract(int32 Index)
{
	// KeyOverlay (툴팁) 위치 설정
	UGridSlot* GridSlot = Cast<UGridSlot>(InteractKeyOverlay->Slot);
	ensure(GridSlot);
	GridSlot->SetRow(Index);
	
	// 해당 인덱스에 포커스를 준다.
	if (Index < InteractButtons.Num())
	{
		// Set Focus 로 하니까, 자꾸 키보드 인풋을 먹는 문제가 있다..
		// 이벤트를 만들어서 BP에서 이벤트 스타일 처리를 해야 할 것 같다..
		//InteractButtons[Index]->SetFocus();
		if (ExSelectedInteractButton)
		{
			ExSelectedInteractButton->OnInteractReleased();
		}
		InteractButtons[Index]->OnInteractSelected();
		ExSelectedInteractButton = InteractButtons[Index];
	}
}

// Key Overlay On/Off
void URBInteractWidget::SetKeyOverlayAvailable(const bool IsAvailable)
{
	if (IsAvailable)
	{
		InteractKeyOverlay->SetVisibility(ESlateVisibility::Visible);
		InteractKeyOverlay->SetIsEnabled(true);
	}
	else
	{
		InteractKeyOverlay->SetVisibility(ESlateVisibility::Hidden);
		InteractKeyOverlay->SetIsEnabled(false);
	}
}