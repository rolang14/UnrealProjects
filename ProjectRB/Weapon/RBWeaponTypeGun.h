// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RBWeaponTypeBase.h"
#include "RBWeaponTypeGun.generated.h"

/**
 * 무기 중에서 Gun 타입의 무기가 되는 기초 클래스
 */
UCLASS()
class PROJECTRB_API ARBWeaponTypeGun : public ARBWeaponTypeBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARBWeaponTypeGun();

public:
	// 각각의 무기에 맞는 공격 방식 수행
	virtual void PerformWeaponFieldAttack() override;

};
