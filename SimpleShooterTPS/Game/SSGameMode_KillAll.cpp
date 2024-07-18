// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SSGameMode_KillAll.h"
#include "Controller/SSAIController.h"
#include "EngineUtils.h"

void ASSGameMode_KillAll::PawnKilled(APawn* KilledPawn) const
{
	Super::PawnKilled(KilledPawn);

	if (KilledPawn == nullptr)
	{
		return;
	}

	// 플레이어가 죽었다면
	APlayerController* PlayerController = Cast<APlayerController>(KilledPawn->GetController());
	if (PlayerController)
	{
		SendGameEndMessage(false);
	}
	// AI가 모두 죽었는지 순회
	else
	{
		for (ASSAIController* AIController : TActorRange<ASSAIController>(GetWorld()))
		{
			if (!AIController->IsDead())
			{
				return;
			}
		}

		SendGameEndMessage(true);
	}
}

void ASSGameMode_KillAll::SendGameEndMessage(const bool bIsPlayerWinnder) const
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		// 플레이어가 이겼는지 졌는지 판단하고 종료함수 호출
		//Controller->GameHasEnded(Controller->GetPawn(), Controller->IsPlayerController() ^ bIsPlayerWinnder ? true : false);
		// XOR 는 그냥 아래처럼 쓰는게 더 직관적임
		// Focus 는 그냥 해당 폰에게 붙여 줌. (타 게임에서는 사망 후 자신을 죽인 폰에게 카메라를 붙이기도 하므로 이는 잘 생각해서 넣으면 된다.)
		Controller->GameHasEnded(Controller->GetPawn(), Controller->IsPlayerController() == bIsPlayerWinnder);
	}
}