// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
// �츮�� ������ ����
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// ������ ���õ� UI ��ҵ��� ��� �� �غ� �Ϸ�Ǹ� NativeConstruct �Լ��� ȣ���� �ȴ�.
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		// ������Ʈ�� Owner�� �� Widget�� Owner �̹Ƿ�
		ABUserWidget->SetOwningActor(this->GetOwner());
	}
}
