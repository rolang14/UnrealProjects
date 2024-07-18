// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RBCharacterFieldAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URBCharacterFieldAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 필드 공격에 대한 인터페이스
 */
class PROJECTRB_API IRBCharacterFieldAttackInterface
{
	GENERATED_BODY()

public:
	// 몽타주 재생 중 특정 타이밍부터는 모션을 끊고 이동 가능하도록 구현
	virtual void InterruptFieldAttack() = 0;
	// 필드 어택이 적중했는지 체크
	virtual void IsFieldAttackHit() = 0;
};
