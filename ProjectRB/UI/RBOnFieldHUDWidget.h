// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBOnFieldHUDWidget.generated.h"

class URBInteractWidget;
class URBDialogueBoxWidget;

/**
 * 필드 상태에서의 HUD 위젯
 */
UCLASS()
class PROJECTRB_API URBOnFieldHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// OnFieldHUDWidget 생성자
	virtual void NativeConstruct() override;
	
public:
	void SetInteractWidgetVisiblity(const ESlateVisibility InVisibility) const;
	void SetDialogueBoxVisibility(const ESlateVisibility InVisibility) const;

	// Interact 처리 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<URBInteractWidget> WBP_Interact;

	// Dialogue 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<URBDialogueBoxWidget> WBP_DialogueBox;
};
