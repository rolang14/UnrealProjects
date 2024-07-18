// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RBOnFieldHUDWidget.h"
#include "UI/RBInteractWidget.h"
#include "UI/RBDialogueBoxWidget.h"

#include "Interface/RBCharacterWidgetInterface.h"


void URBOnFieldHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// HUD Widget 은 BeginPlay 이후에 생성되므로,
	// 이 위젯이 소유한 블루프린트를 참조하는 다른 위젯들은 이미 에디터 단에서 생성자가 모두 반영되어있다. (당연하게도, 블루프린트 객체가 이미 해당 클래스를 부모로 하여 생성되어있다는 가정 하에, 이미 생성되어 있으므로)
	// 따라서 현재는 생성자 호출 시점에 대해 고려할 필요가 없다.
	//InteractWidget = Cast<URBInteractWidget>(GetWidgetFromName(TEXT("WBP_Interact")));
	ensure(WBP_Interact);
	// 생성자 호출이 완료된 뒤, 캐릭터 위젯 셋업을 통해 마무리 세팅을 해 준다.
	if (IRBCharacterWidgetInterface* CharacterWidgetInterface = Cast<IRBCharacterWidgetInterface>(GetOwningPlayerPawn()))
	{
		CharacterWidgetInterface->SetupInteractWidget(WBP_Interact);
	}

	ensure(WBP_DialogueBox);
	// For Test
	WBP_DialogueBox->SetVisibility(ESlateVisibility::Hidden);
	WBP_DialogueBox->OnVisibilityChanged.AddDynamic(WBP_DialogueBox, &URBDialogueBoxWidget::PlayFadeInAnimation);
	//WBP_DialogueBox->SetVisibility(ESlateVisibility::Visible);
	//WBP_DialogueBox->SetSpeakerText(FText::FromString(TEXT("나")));
	//WBP_DialogueBox->SetScriptText(FText::FromString(TEXT("안녕하세요.")));
}

void URBOnFieldHUDWidget::SetInteractWidgetVisiblity(const ESlateVisibility InVisibility) const
{
	WBP_Interact->SetVisibility(InVisibility);
}

void URBOnFieldHUDWidget::SetDialogueBoxVisibility(const ESlateVisibility InVisibility) const
{
	if (InVisibility == ESlateVisibility::Hidden)
	{
		WBP_DialogueBox->CollapseDialogue();
	}
	else
	{
		WBP_DialogueBox->SetVisibility(InVisibility);
	}
}
