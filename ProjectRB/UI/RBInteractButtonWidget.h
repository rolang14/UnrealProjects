// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RBInteractButtonWidget.generated.h"


class USizeBox;
class UButton;
class UTextBlock;

/**
 * 상호작용 버튼 정의 위젯
 */
UCLASS()
class PROJECTRB_API URBInteractButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void DestructInteract();

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	FORCEINLINE uint8 IsDestructing() const { return bIsProcessingDestruct; }

	FORCEINLINE void SetDisplayText(const FText DisplayText);
	FORCEINLINE const FText GetDisplayText() const;

	// BP의 버튼 애니메이션 불러오기. 이 이름은 반드시 위젯의 Anim 애셋 이름과 동일해야 한다.
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> InteractButtonCreated_Anim;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> InteractButtonDestruct_Anim;
	
	// Focus 대신 사용할 커스텀 이벤트 (버튼 하이라이트용)
	UFUNCTION(BlueprintImplementableEvent, Category = "Focus")
	void OnInteractSelected();

	UFUNCTION(BlueprintImplementableEvent, Category = "Focus")
	void OnInteractReleased();

private:
	// 애니메이션 종료 후 삭제를 위한 델리게이트
	FWidgetAnimationDynamicEvent OnDestructAnimFinished;

	// 위젯 삭제용 함수
	UFUNCTION()
	void ProcessDestruct();

	UPROPERTY()
	TObjectPtr<USizeBox> ButtonSizeBox;

	UPROPERTY()
	TObjectPtr<UButton> InteractButton;

	// 상호작용 표시 텍스트
	UPROPERTY()
	TObjectPtr<UTextBlock> InteractText;

	// 파괴 상태에 있다는 것을 외부에 알려주기 위함
	uint8 bIsProcessingDestruct : 1;
};
