// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

// 블루프린트에서 사용이 가능한 열거형 선언
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
	
// Asset Manager 를 위한 태그 지정
public:
	// 부모 함수 오버라이드
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// 보통 이렇게 AssetManager 에서 지정한 이름 + GetFName() 을 하여 아이디를 만든다고 함.
		return FPrimaryAssetId("ABItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
	
};
