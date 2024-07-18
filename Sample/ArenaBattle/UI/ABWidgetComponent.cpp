// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
// 우리가 소유할 위젯
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 위젯에 관련된 UI 요소들이 모두 다 준비가 완료되면 NativeConstruct 함수가 호출이 된다.
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		// 컴포넌트의 Owner가 즉 Widget의 Owner 이므로
		ABUserWidget->SetOwningActor(this->GetOwner());
	}
}
