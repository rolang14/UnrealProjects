// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RBEnemyEncounterData.generated.h"


UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Normal = 0,
	Elite = 1,
	Boss = 2
};


USTRUCT(BlueprintType)
struct FRBEnemyEncounterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FRBEnemyEncounterData() : Type(EEnemyType::Normal), EnemyName(FName(NAME_None)), Level(0) {}
	FRBEnemyEncounterData(EEnemyType InType, FName InEnemyName, uint8 InLevel) : Type(InType), EnemyName(InEnemyName), Level(InLevel) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Type")
	EEnemyType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Type")
	FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Type")
	uint8 Level;

};