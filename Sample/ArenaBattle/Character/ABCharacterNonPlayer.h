// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Interface/ABCharacterAIInterface.h"
#include "Engine/StreamableManager.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
// INI ������ �����ϱ� ����
UCLASS(config=ArenaBattle)
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

protected:
	// NPC Mesh �� �ε�� ����
	virtual void PostInitializeComponents() override;

protected:
	// NPC�� �׾ �������� ���� �������̵���.
	void SetDead() override;
	// NPC �޽� �ε尡 �Ϸ�Ǹ� ȣ��� �Լ�
	void NPCMeshLoadCompleted();

	// �� ����� INI���Ͽ��� ���� �ҷ���.
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	// Skeletal Mesh�� ������ ���(NPCMeshes) ���� �񵿱������� �ε��ϱ� ����.
	TSharedPtr<FStreamableHandle> NPCMeshHandle;

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRadius() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	// ��������Ʈ ����
	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
