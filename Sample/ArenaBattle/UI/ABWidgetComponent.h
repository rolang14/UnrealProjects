// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ABWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	// InitWidget �� ȣ���� �Ǹ�, ������ ���� �ν��Ͻ��� �� ������ �Ϸ�Ǿ��ٰ� ����. (BeginPlay �����̹Ƿ�)
	virtual void InitWidget() override;
	
};
