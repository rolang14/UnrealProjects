// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBDialogueBoxWidget.generated.h"


class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTRB_API URBDialogueBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	// Set Dialogue's Speaker Text.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSpeakerText(const FText InSpeaker);

	// Set Dialogue's Script Text.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetScriptText(const FText InScript);
	
	UFUNCTION()
	void PlayFadeInAnimation(ESlateVisibility InVisibility);

	// 반드시 이 함수를 사용하여 외부에서 이 위젯을 Collapse 시켜야 한다.
	void CollapseDialogue();

	// 현재 타이핑 중인지.
	FORCEINLINE const uint8 IsTyping() const { return bIsTyping; }

protected:
	// Fade Out 이후 Collapsed 시키기 위한 델리게이트
	FWidgetAnimationDynamicEvent OnFadeOutAnimFinished;

	// Fade Out -> Collapse 순서로 실행하기 위한 UI 숨기기 함수
	// 문서를 보니, Hidden 은 Collapsed 보다 코스트가 높다고 한다.
	// Collapsed 되면 Prepass 계산을 무시하고 Tick, Animation 등의 계산을 하지 않고,
	// Hidden 되어도 여전히 공간을 차지하기 때문에, 만약 반드시 해당 공간을 차지하고 있어야 하는 UI라면 Hidden 을 쓰는 것이 맞겠지만
	// 꼭 자리를 차지하고 있어야 하는 위젯이 아니라면 Collapsed 를 사용하자.
	UFUNCTION()
	void SetCollapsed();

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DialogueFadeIn_Anim;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DialogueFadeOut_Anim;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Speaker;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Script;

	FString FullScript;
	FString CurrentScript;
	int32 ScriptIndex;

	uint8 bIsTyping : 1;
	float TypeTime;
};
