// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RBDialogueBoxWidget.h"
#include "Components/TextBlock.h"


void URBDialogueBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsTyping = false;
	Txt_Speaker->SetText(FText());
	Txt_Script->SetText(FText());

	// Fade Out -> Hidden 바인딩
	OnFadeOutAnimFinished.BindDynamic(this, &URBDialogueBoxWidget::SetCollapsed);
	BindToAnimationFinished(DialogueFadeOut_Anim, OnFadeOutAnimFinished);
}

void URBDialogueBoxWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Script 를 타이핑한다.
	if (bIsTyping && CurrentScript != FullScript)
	{
		TypeTime += InDeltaTime;
		if (TypeTime > 0.05f)
		{
			CurrentScript = FullScript.Mid(0, ScriptIndex++);
			Txt_Script->SetText(FText::FromString(CurrentScript));
			TypeTime = 0;
		}

		if (CurrentScript == FullScript)
		{
			bIsTyping = false;
		}
	}
}

// Speaker 텍스트 Set
void URBDialogueBoxWidget::SetSpeakerText(const FText InSpeaker)
{
	Txt_Speaker->SetText(InSpeaker);
}

// Script 텍스트 Set
void URBDialogueBoxWidget::SetScriptText(const FText InScript)
{
	FullScript = InScript.ToString();
	CurrentScript = FString();

	TypeTime = 0.0f;
	ScriptIndex = 0;
	bIsTyping = true;
}

// Visible 상태에 진입 시 FadeInAnimation 재생
void URBDialogueBoxWidget::PlayFadeInAnimation(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		PlayAnimationForward(DialogueFadeIn_Anim);
	}
}

// 외부에서는 반드시 이 함수를 통해 위젯을 Hide 시켜야 한다. 
void URBDialogueBoxWidget::CollapseDialogue()
{
	PlayAnimationForward(DialogueFadeOut_Anim);
}

// DialogueFadeOut_Anim 종료 시 호출되는 바인딩 함수
void URBDialogueBoxWidget::SetCollapsed()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
