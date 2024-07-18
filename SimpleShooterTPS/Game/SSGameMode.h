// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// 폰이 죽으면 호출될 함수 (게임의 승패 판단용)
	virtual void PawnKilled(APawn* KilledPawn) const;
};
