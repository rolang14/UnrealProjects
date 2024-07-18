// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/MenuInterface.h"

#include "MenuWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 위젯 소멸자
	virtual void NativeDestruct() override;

	// 기본적인 버튼 이벤트 동적 바인딩은 여기서 수행해야 한다.
	virtual bool Initialize() override;

public:
	// 메뉴 인터페이스 설정
	FORCEINLINE void SetMenuInterface(IMenuInterface* InMenuInterface) { MenuInterface = InMenuInterface; }

	// 기본 메뉴 진입 설정
	void SetUp();

protected:
	void ClearWidget();

	IMenuInterface* MenuInterface;
};
