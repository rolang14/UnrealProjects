// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBInteractWidget.generated.h"


class URBInteractButtonWidget;
class UGridPanel;
class UOverlay;
class UTextBlock;

class URBInteractData;

/**
 * 상호작용 버튼을 갖고, 상호작용 인터페이스를 정의하는 위젯
 */
UCLASS()
class PROJECTRB_API URBInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Interact Data List 에 맞춰 상호작용 버튼 생성/삭제
	void UpdateInteractButton(const TArray<const URBInteractData*>& InteractDataList);
	void CreateInteractButton(const FText DisplayMessage, const int32 Index);
	void RemoveInteractButton(const int32 Index);

	// Selected Interact 에 맞춰 Interact Press Key 및 선택 효과 위치 변경
	void UpdateSelectedInteract(int32 Index);
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Interact Widget")
	TSubclassOf<URBInteractButtonWidget> InteractButtonWidgetClass;

	UPROPERTY()
	TObjectPtr<UGridPanel> InteractGridPanel;

	UPROPERTY()
	TObjectPtr<UOverlay> InteractKeyOverlay;

	UPROPERTY()
	TObjectPtr<UTextBlock> InteractKeyText;

private:
	// Interact Press Key On/Off
	void SetKeyOverlayAvailable(const bool IsAvailable);

	UPROPERTY()
	TArray<TObjectPtr<URBInteractButtonWidget>> InteractButtons;

	UPROPERTY()
	TObjectPtr<URBInteractButtonWidget> ExSelectedInteractButton;
};
