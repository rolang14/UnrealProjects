// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "Engine/AssetManager.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// Mesh가 로딩되기 전까지는 숨기기.
	GetMesh()->SetHiddenInGame(true);

	// NonPlayer 에 AI 컨트롤러를 적용
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AABAIController::StaticClass();
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 체크하고
	ensure(NPCMeshes.Num() > 0);
	// 인덱스를 랜덤으로 뽑아서
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	// 애셋 매니저를 통해 UObject 생성.
	// AsnycLoad가 끝나는 시점의 델리게이트에 NPCMeshLoadCompleted 를 바인딩함.
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 사망 후에도 NPC가 계속해서 플레이어를 공격하는 현상 수정
	AABAIController* AIController = Cast<AABAIController>(GetController());
	if (AIController)
	{
		// 죽은 후, AIController 를 가져와서 StopAI 를 실행해주면 된다.
		AIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	// 멤버 함수(사라지기)는 람다함수로 구현해서 바로 델리게이트에 꽂아보자.
	// 이렇게 람다 함수를 만들면 바로 TimerDelegate에 연결되면서 타이머가 발동되면 이 함수가 호출되게 되는 것이다.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);

}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	// 체크
	if (NPCMeshHandle.IsValid())
	{
		// 핸들에서 로드된 Asset을 가져와서 캐스팅.
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			// Mesh를 로드하고
			GetMesh()->SetSkeletalMesh(NPCMesh);
			// Mesh가 로드되면 보이도록 설정
			GetMesh()->SetHiddenInGame(false);
		}
	}

	// 설정이 다 끝나면 Release
	NPCMeshHandle->ReleaseHandle();
}

float AABCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AABCharacterNonPlayer::GetAIDetectRadius()
{
	return 400.0f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 0.0f;
}

void AABCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AABCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

// 콤보액션 종료 후에 호출됨. -> CharacterBase
void AABCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
