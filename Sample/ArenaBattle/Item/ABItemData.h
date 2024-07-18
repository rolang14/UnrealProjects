// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

// �������Ʈ���� ����� ������ ������ ����
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
// Asset Manager �� ���� �±� ����
public:
	// �θ� �Լ� �������̵�
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// ���� �̷��� AssetManager ���� ������ �̸� + GetFName() �� �Ͽ� ���̵� ����ٰ� ��.
		return FPrimaryAssetId("ABItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
	
};
