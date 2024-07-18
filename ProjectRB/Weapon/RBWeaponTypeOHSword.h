// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RBWeaponTypeBase.h"
#include "RBWeaponTypeOHSword.generated.h"

/**
 * 무기 중에서 One Handed Sword 타입의 무기가 되는 기초 클래스
 */
UCLASS()
class PROJECTRB_API ARBWeaponTypeOHSword : public ARBWeaponTypeBase
{
	GENERATED_BODY()
	
public:
	ARBWeaponTypeOHSword();

public:
	virtual void PerformWeaponFieldAttack() override;

};
