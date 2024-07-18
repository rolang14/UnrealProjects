// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RBWeaponFieldAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URBWeaponFieldAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 무기 타입에 따른 필드 공격 구현용 (Ex: 총 타입이라면 발사체를 발사함)
 */
class PROJECTRB_API IRBWeaponFieldAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 해당 무기의 필드 어택 인터페이스
	virtual void PerformWeaponFieldAttack() = 0;
	// 필드 공격을 수행할 때 무기 소환 이펙트 재생
	virtual void PerformWeaponSummonEffect() = 0;
	// 필드 공격을 수행 후 무기 소멸 이펙트 재생
	virtual void PerformWeaponDisappearEffect() = 0;
};
