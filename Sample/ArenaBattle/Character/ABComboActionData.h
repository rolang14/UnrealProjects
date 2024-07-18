// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

	// 몽타주 섹션 이름
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// 총 몇개의 콤보 액션이 존재하는지 (4개)
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	// 입력 기준 프레임
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;

	// 프레임의 기준 재생 속도를 지정해서 정확한 타이밍에 체크가 일어나도록 함.
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

protected:

private:
	
};
