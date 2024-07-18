// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RBWeaponTypeOHSword.h"

ARBWeaponTypeOHSword::ARBWeaponTypeOHSword()
{
	WeaponType = WEAPON_TYPE::OHSWORD;
}

void ARBWeaponTypeOHSword::PerformWeaponFieldAttack()
{
	Super::PerformWeaponFieldAttack();

	UE_LOG(LogTemp, Display, TEXT("Perform Weapon Field Attack From Gun."));
}
