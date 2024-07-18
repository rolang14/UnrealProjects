// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/TTBasePawn.h"
#include "TTTowerPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANK_API ATTTowerPawn : public ATTBasePawn
{
	GENERATED_BODY()
	
public:
	ATTTowerPawn();
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void HandleDestruction() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float FireRange = 1000.f;

	TObjectPtr<class ATTTankPawn> TankPawn;

	FTimerHandle FireTimerHandle;

	void CheckFireCondition();
	bool CheckPlayerInRange() const;
	const float GetPlayerDist() const;
};
