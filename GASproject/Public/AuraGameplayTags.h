// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura Gameplay Tags
 * 
 * Singleton for containing native Gameplay Tags.
 */
struct FAuraGameplayTags
{
public:
	// 싱글톤 Getter
	FORCEINLINE static const FAuraGameplayTags& Get() { return GameplayTags; }
	// 싱글톤 초기화
	static void InitializeNativeGameplayTags();

#pragma region Primary Attributes Tag

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Dexterity;

#pragma endregion

#pragma region Secondary Attributes Tag

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_ManaRegeneration;

#pragma endregion

#pragma region Input Tag

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

#pragma endregion

#pragma region Damage & Effect Tag

	FGameplayTag Damage;
	// Hit React 를 위한 Effect Tag
	FGameplayTag Effects_HitReact;

#pragma endregion


#pragma region Message Tag

	FGameplayTag Message_HealthPotion;
	FGameplayTag Message_HealthCrystal;
	FGameplayTag Message_ManaPotion;
	FGameplayTag Message_ManaCrystal;

#pragma endregion

private:
	// 싱글톤 인스턴스 (Static)
	static FAuraGameplayTags GameplayTags;
};