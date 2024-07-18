// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RBWeaponTypeGun.h"

// Sets default values
ARBWeaponTypeGun::ARBWeaponTypeGun()
{
	WeaponType = WEAPON_TYPE::GUN;

}

void ARBWeaponTypeGun::PerformWeaponFieldAttack()
{
	Super::PerformWeaponFieldAttack();

	UE_LOG(LogTemp, Display, TEXT("Perform Weapon Field Attack From Gun."));
}

