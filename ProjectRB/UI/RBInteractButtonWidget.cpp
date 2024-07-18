// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RBInteractButtonWidget.h"

#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Animation/WidgetAnimation.h"


void URBInteractButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Seems Deprecated
	//bIsFocusable = true;
	SetIsFocusable(true);
	bIsProcessingDestruct = false;

	ButtonSizeBox = Cast<USizeBox>(GetWidgetFromName(TEXT("ButtonSizeBox")));
	ensure(ButtonSizeBox);

	InteractButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Interact")));
	ensure(InteractButton);

	InteractText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Interact")));
	ensure(InteractText);

	// 버튼 생성 시 애니메이션 호출
	PlayAnimationForward(InteractButtonCreated_Anim);

	// 버튼 제거 애니메이션 종료 시 이 위젯이 삭제되도록 델리게이트 바인딩
	OnDestructAnimFinished.BindDynamic(this, &URBInteractButtonWidget::ProcessDestruct);
	BindToAnimationFinished(InteractButtonDestruct_Anim, OnDestructAnimFinished);
}

void URBInteractButtonWidget::DestructInteract()
{
	// Seems Deprecated
	//bIsFocusable = false;
	SetIsFocusable(false);
	bIsProcessingDestruct = true;
	PlayAnimationForward(InteractButtonDestruct_Anim);
}

void URBInteractButtonWidget::ProcessDestruct()
{
	RemoveFromParent();
	// 메모리 누수 방지용
	// 이것때문인지? 실험해보니 삭제된 위젯은 GC에 의해서 잘 수거가 된다. << 아마 그냥 RemoveFromParent 만 해도 더이상 참조되지 않으므로 GC에 의해 수집되는 것 같다.
	// 향후 메모리 관련 문제가 생길 경우 갱신될 때마다 위젯 삭제/생성이 아니라, 그냥 버튼 MAX(4)개를 두고 내용만 바꾸면서 애니메이션 재생하는 방식으로 수정.
	Destruct();
}

void URBInteractButtonWidget::SetDisplayText(const FText DisplayText)
{
	InteractText->SetText(DisplayText);
}

const FText URBInteractButtonWidget::GetDisplayText() const
{
	return InteractText->GetText();
}