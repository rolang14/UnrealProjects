// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/SSGameMode.h"
#include "SSGameMode_KillAll.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASSGameMode_KillAll : public ASSGameMode
{
	GENERATED_BODY()
	
public:
	// 폰이 죽었을 때 게임 승패 판정을 위해 호출될 함수
	virtual void PawnKilled(APawn* KilledPawn) const override;
	
private:
	// 게임 종료 시 각 컨트롤러에게 게임 종료 Broadcast
	void SendGameEndMessage(const bool bIsPlayerWinnder) const;

};
