// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	// Creator
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// 내부의 ProgressBar 포인터 획득용
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

	// Widget의 ProgressBar (HpBar) 업데이트용
	void UpdateHpBar(const float NewCurrentHp);

	// Stat이 업데이트 되었을 때 Hp Text Block Text 생성용
	FString GetHpStatText();

protected:
	// UI 컨트롤
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpTextBlock;

	// 최대 Hp : 스탯에도 존재하지만 갖다 쓰기 위해 추가함.
	UPROPERTY()
	float MaxHp;

	// 현재 Hp
	UPROPERTY()
	float CurrentHp;
};
