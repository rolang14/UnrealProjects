// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "Engine/AssetManager.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// Mesh�� �ε��Ǳ� �������� �����.
	GetMesh()->SetHiddenInGame(true);

	// NonPlayer �� AI ��Ʈ�ѷ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AABAIController::StaticClass();
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// üũ�ϰ�
	ensure(NPCMeshes.Num() > 0);
	// �ε����� �������� �̾Ƽ�
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	// �ּ� �Ŵ����� ���� UObject ����.
	// AsnycLoad�� ������ ������ ��������Ʈ�� NPCMeshLoadCompleted �� ���ε���.
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// ��� �Ŀ��� NPC�� ����ؼ� �÷��̾ �����ϴ� ���� ����
	AABAIController* AIController = Cast<AABAIController>(GetController());
	if (AIController)
	{
		// ���� ��, AIController �� �����ͼ� StopAI �� �������ָ� �ȴ�.
		AIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	// ��� �Լ�(�������)�� �����Լ��� �����ؼ� �ٷ� ��������Ʈ�� �Ⱦƺ���.
	// �̷��� ���� �Լ��� ����� �ٷ� TimerDelegate�� ����Ǹ鼭 Ÿ�̸Ӱ� �ߵ��Ǹ� �� �Լ��� ȣ��ǰ� �Ǵ� ���̴�.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);

}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	// üũ
	if (NPCMeshHandle.IsValid())
	{
		// �ڵ鿡�� �ε�� Asset�� �����ͼ� ĳ����.
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			// Mesh�� �ε��ϰ�
			GetMesh()->SetSkeletalMesh(NPCMesh);
			// Mesh�� �ε�Ǹ� ���̵��� ����
			GetMesh()->SetHiddenInGame(false);
		}
	}

	// ������ �� ������ Release
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

// �޺��׼� ���� �Ŀ� ȣ���. -> CharacterBase
void AABCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
