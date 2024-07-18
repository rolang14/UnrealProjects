// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TTGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANK_API ATTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void SetActorDead(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void GameWon(bool bIsGameWon);
	
private:
	TObjectPtr<class ATTTankPawn> TankPlayer;
	TObjectPtr<class ATTPlayerController> TankPlayerController;

	FTimerHandle StartDelayTimerHandle;
	float StartDelay = 3.f;

	int32 NumOfTotalEnemy = 0;
	const int32 GetTowerEnemyNumber();
};
