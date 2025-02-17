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

	// 부모 함수 오버라이드
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// 보통 이렇게 AssetManager 에서 지정한 이름 + GetFName() 을 하여 아이디를 만든다고 함.
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat BaseStat;
};
