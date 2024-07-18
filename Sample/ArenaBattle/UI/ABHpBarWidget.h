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
	// ������ ProgressBar ������ ȹ���
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

	// Widget�� ProgressBar (HpBar) ������Ʈ��
	void UpdateHpBar(const float NewCurrentHp);

	// Stat�� ������Ʈ �Ǿ��� �� Hp Text Block Text ������
	FString GetHpStatText();

protected:
	// UI ��Ʈ��
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpTextBlock;

	// �ִ� Hp : ���ȿ��� ���������� ���� ���� ���� �߰���.
	UPROPERTY()
	float MaxHp;

	// ���� Hp
	UPROPERTY()
	float CurrentHp;
};
