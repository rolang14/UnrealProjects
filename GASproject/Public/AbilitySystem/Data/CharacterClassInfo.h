// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"


class UGameplayEffect;
class UGameplayAbility;


UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior = 0,
	Ranger,
	Elementalist
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
public:
	// 초기 Primary Attributes 는 서로 다르지만
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Look up for CharacterClassInfo
	const FCharacterClassDefaultInfo& GetClassDefaultInfo(ECharacterClass InCharacterClass) const;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Class Default Info")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;

	// Primary 말고 Secondary, Vital 이 적용되는 방식은 똑같기 때문에 공통으로 사용한다.
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	// 모든 클래스가 공통으로 가질 어빌리티 (피격, 사망 등)
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommomAbilities;

	// 대미지 공식 관련 계수 정의
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
