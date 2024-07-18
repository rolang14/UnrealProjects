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
	// InitWidget 가 호출이 되면, 위젯에 대한 인스턴스는 다 생성이 완료되었다고 본다. (BeginPlay 이후이므로)
	virtual void InitWidget() override;
	
};
