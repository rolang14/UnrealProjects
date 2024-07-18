// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABScrollItemData : public UABItemData
{
	GENERATED_BODY()
	
public:
	UABScrollItemData();

	// �θ� �Լ� �������̵�
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// ���� �̷��� AssetManager ���� ������ �̸� + GetFName() �� �Ͽ� ���̵� ����ٰ� ��.
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat BaseStat;
};
