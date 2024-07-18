// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"


// ATTRIBUTE_ACCESSORS 매크로를 통해서 특정 Attribute에 대한 기본적인 Getter, Setter, Initter 를 자동으로 만들 수 있다.
// Attribute 들은 각각이 구조체이기 때문에 게터 세터를 만드는게 좀 귀찮아서 이런식으로 사용한다고 함. 그냥 Attribute 에 대한 Boilerplate 라고 생각하면 된다.
// 이걸 사용하려면 AuraAbilitySystemComponent 헤더가 필요하므로 이건 어쩔 수 없이 헤더파일에서 포함해야 한다.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


// GE Property 정보를 담기 위한 구조체
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

public:
	FEffectProperties() {}
	
public:
	FGameplayEffectContextHandle GEContextHandle;

#pragma region Source

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

#pragma endregion

#pragma region Target

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

#pragma endregion

};


// AttributeFunction 델리게이트 대신 델리게이트 인스턴스 래퍼 탬플릿을 사용한다. 결론적으로 FGameplayAttribute를 반환하는 함수 포인터 타입인 셈이다.
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
// 
// 위의 타입을 탬플릿으로 작성하면 아래와 같다. 반환 타입이 T인 함수 포인터를 아래처럼 정의한다. 이렇게 하면 굳이 FGameplayAttribute() 에만 국한되지 않고 모든 함수에 Generic 하게 사용 가능하다.
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

#pragma region Replicate & Attributes

public:
	/** Returns properties that are replicated for the lifetime of the actor channel */
	// 원 주석과 같이, 특정 멤버를 Replicate 하기 위해서는 클래스 내에 아래의 함수를 구현하고, 이 액터 채널에 Lifttime동안 Replicated 되길 원하는 Property를 추가해야 한다. 
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

public:
	// Tag와 Attribute 반환 델리게이트를 묶어, 외부에서 모든 Attribute 를 얻을 수 있도록 한다.
	// TBaseStaticDelegateInstance 는 RetVal 델리게이트 대신, 델리게이트 없이도 그냥 바로 함수를 넣기 위해 사용한다. (더 간단해진다는 것만 알면 된다)
	//TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	// 저 복잡한 타입을 그냥 아래처럼 직관적으로 써도 된다.
	//TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributes;
	// 근데 그냥 alias 로 설명을 적어놓고 그냥 명확하게 명명해서 사용하는게 제일 낫다고 한다.
	//TMap<FGameplayTag, FAttributeFuncPtr> TagsToAttributes;
	// 위를 다시 탬플릿 함수 포인터로 만들어서 아래처럼 쓰면 어떤 타입에도 대응이 된다.
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	// Vital Attributes
	// 
	// Health : 체력
	// MaxHealth : 최대체력
	// Mana : 마나
	// MaxMana : 마나

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	// OnRep 함수의 경우, 인자가 0개일 수도, 1개일 수도 있으며 내부적으로 정해져 있다.
	// 0개인 경우 : 그냥 0개임. 콜백을 통해 Rep되었음을 알려주기만 한다.
	// 1개인 경우 : 동일하게 FGameplayAttributeData 를 인자로 사용해서 Old Value 를 넘겨준다. 이를 통해서 새 값(Rep되었으므로 이미 새 값임)과 이전 값을 비교하거나 원하는 작업을 할 수 있다.

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	// Primary Attributes
	//
	// Strength : 힘 (물리 대미지)
	// Intelligence : 지능 (마법 대미지)
	// Resilience : 아머 (대미지 경감)
	// Vigor : 체력 (HP)
	// Dexterity : 민첩 (크리티컬)

	UPROPERTY(ReplicatedUsing = OnRep_Strength, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(ReplicatedUsing = OnRep_Resilience, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(ReplicatedUsing = OnRep_Vigor, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	UPROPERTY(ReplicatedUsing = OnRep_Dexterity, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Dexterity);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	// Secondary Attributes
	//
	// (Res) Armor : 방어력(대미지 경감), Block Chance 를 늘린다.
	// (Res) Armor Penetration : 방어 관통. 적의 방어력을 n% 무시한다.
	// (Armor) Block Chance : 적의 공격을 가드, 경감률은 최종 대미지의 50%이다.
	// (Armor) Critical Hit Resistance : 크리티컬 저항, 적의 크리티컬 '확률'에 저항한다.
	// (Dexterity) Critical Hit Chance : 크리티컬 확률, 확률에 따라 2배(+Crit Hit Damage)의 대미지를 입힌다.
	// (Dexterity) Critical Hit Damage : 크리티컬 대미지, 기존의 2배 크리티컬 대미지에 추가 비율로 대미지를 입힌다.
	// (Vigor) Max Health : 최대 체력.
	// (Vigor) Health Regeneration : 체력 재생, 1초에 일정 양만큼 체력을 자동 회복한다.
	// (Intelligence) Max Mana : 최대 마나.
	// (Intelligence) Mana Regeneration : 마나 재생, 1초에 일정 양만큼 마나를 자동 회복한다.

	UPROPERTY(ReplicatedUsing = OnRep_Armor, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(ReplicatedUsing = OnRep_ArmorPenetration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(ReplicatedUsing = OnRep_BlockChance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitResistance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitChance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitDamage, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(ReplicatedUsing = OnRep_HealthRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UPROPERTY(ReplicatedUsing = OnRep_ManaRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	// Meta Attributes
	//
	// IncomingDamage : 입는 대미지

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

#pragma endregion

#pragma region For Managing Attribute Change

public:
	// 이 함수는 어트리뷰트에 수정이 적용되기 직전에 적용되며, 주로 "Health = Clamp(Health, 0, MaxHealth)" 이런식으로 Clamp 하라고 있는 것이라고 함.
	// 다른 비즈니스 로직을 여기서 수행하면 안된다. Just Clamping.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 위의 콜백 함수보다는 아래 콜백 함수가 더 사용하기에 좋음.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	// For Setting up GE Properties
	void SetupEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutEffectProperties) const;

#pragma endregion

// Let Player Show Incoming Damage
private:
	void ShowDamageNumber(const FEffectProperties& EffectProperties, const float LocalIncomingDamage) const;




};
