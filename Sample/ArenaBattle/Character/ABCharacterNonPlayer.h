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
// INI 파일을 참조하기 위함
UCLASS(config=ArenaBattle)
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

protected:
	// NPC Mesh 가 로드될 시점
	virtual void PostInitializeComponents() override;

protected:
	// NPC가 죽어서 없어지기 위해 오버라이드함.
	void SetDead() override;
	// NPC 메시 로드가 완료되면 호출될 함수
	void NPCMeshLoadCompleted();

	// 이 멤버는 INI파일에서 값을 불러옴.
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	// Skeletal Mesh를 지정된 경로(NPCMeshes) 에서 비동기적으로 로드하기 위함.
	TSharedPtr<FStreamableHandle> NPCMeshHandle;

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRadius() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	// 델리게이트 변수
	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
