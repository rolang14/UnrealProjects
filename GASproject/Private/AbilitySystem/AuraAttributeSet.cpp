// Copyright NHE


#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerController.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();

	// Primary Attributes
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);

	// Secondary Attributes
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(AuraGameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);

}

#pragma region Replicate

// 이 액터의 Replicated Property 추가
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 클래스 이름
	// Property 이름
	// Replicate 될 조건 (COND_None 는 아무 조건 없이 항상 REP 하길 원한다는 것, 나머지 조건은 알아서 찾아볼 것)
	// Notify 조건 (Always = 서버에서 값을 Set 하면 반드시 Notify 한다, OnChanged = 서버에서 값을 Set 했지만 이전 값과 동일하다면 Notify 하지 않는다.)
	// 을 통해서 Replicate 되도록 등록한다.

	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

// GetLifetimeReplicatedProps 을 통해 Property 를 Replicate 되도록 등록하고, 등록한 조건에 따라서 Replicate 가 일어나면 아래 콜백 함수가 호출된다.
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// 이 내부에서는 아래 매크로를 사용하여 명시적으로 Notify 해주어야만 한다.
	// Delegate 개념?
	// 여기도 동일하게 클래스 이름, Property Name, OldValue 를 사용한다.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Dexterity, OldDexterity);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana)
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

#pragma endregion

#pragma region For Managing Attribute Change

// 이 함수는 반드시 Attribute 가 변경되기 직전 한번 호출된다. Clamping 용도로 사용한다.
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 단순히 New Value (GE Modifier 에서 Querying 된 값) 만을 변경함.
	// 그렇기 때문에 Modifier 가 실제로 적용되는 값에는 영향을 미치지 않는다.
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

// GE 정보를 아래와 같이 획득할 수 있다.
void UAuraAttributeSet::SetupEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutEffectProperties) const
{
	// Source = causer of the effect (적용된 GE를 일으킨 액터), Target = target of the effect (owner of this AS) (지금 Attribute 가 변경되는 이놈)
	// 
	// Source 를 가져오기 위해서는 아래처럼 할 수도 있다.
	OutEffectProperties.GEContextHandle = Data.EffectSpec.GetContext();
	OutEffectProperties.SourceASC = OutEffectProperties.GEContextHandle.GetInstigatorAbilitySystemComponent();

	if (IsValid(OutEffectProperties.SourceASC) && OutEffectProperties.SourceASC->AbilityActorInfo.IsValid() && OutEffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// Source의 아바타 액터
		// Avatar 와 Owner Actor 는 확실한 차이가 있으므로 반드시 명시한다.
		OutEffectProperties.SourceAvatarActor = OutEffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();

		// Source의 컨트롤러 (없을 수도 있다. 포션같은 경우에는 컨트롤러가 딱히 없자늠)
		OutEffectProperties.SourceController = OutEffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (OutEffectProperties.SourceController == nullptr && OutEffectProperties.SourceAvatarActor != nullptr)
		{
			// 만약 Controller를 가져오지 못했다면 Pawn으로 캐스팅하여 직접 가져올 수도 있다.
			if (const APawn* Pawn = Cast<APawn>(OutEffectProperties.SourceAvatarActor))
			{
				OutEffectProperties.SourceController = Pawn->GetController();
			}
		}

		// Source의 캐릭터도 얻어올 수 있다 (있다면)
		if (OutEffectProperties.SourceController)
		{
			OutEffectProperties.SourceCharacter = Cast<ACharacter>(OutEffectProperties.SourceController->GetPawn());
		}
	}

	// Target의 아바타 액터
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutEffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		OutEffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		OutEffectProperties.TargetCharacter = Cast<ACharacter>(OutEffectProperties.TargetAvatarActor);
		OutEffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutEffectProperties.TargetAvatarActor);
	}
}

// GE가 어떻게 적용되었는지 확인할 수 있다. 서버에서만 실행된다.
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	// 이렇게 하면 FGameplayEffectModCallbackData 로부터 얻은 데이터를 간단하게 사용할 수 있다.
	FEffectProperties EffectProperties;
	SetupEffectProperties(Data, EffectProperties);

	// Actual Clamping
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// "실제로" Health 에 값을 적용한다. 이미 GE가 적용된 이후이므로 Modifier 에 의해서 변경될 일이 없다.
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Error, TEXT("Changed Health on TargetActor : %s, Health : %f"), *EffectProperties.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// "실제로" Health 에 값을 적용한다. 이미 GE가 적용된 이후이므로 Modifier 에 의해서 변경될 일이 없다.
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		// (Meta Attribute 의 경우 동일) Incoming Damage 를 로컬로 캐싱하고 Attribute 는 0으로 만든다.
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		// 계산 후, 실질적으로 대미지가 발생한 경우
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bIsDead = GetHealth() <= 0.f;
			// 대미지가 있고, 사망했다면
			if (bIsDead)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
				{
					CombatInterface->PerformDeath();
				}
			}
			// 대미지가 있고, 죽지 않았다면 Hit React Ability를 발동한다.
			else
			{
				// 태그를 넘기기 위해서는 이렇게 TagContainer 에 태그를 담아서 던져야 한다.
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				// Tag 를 통해 해당 태그가 있는 Ability 를 발동시키려고 시도한다. (ServerSide)
				// Ability 를 직접 발동하는 것보다 Tag를 통해 발동시키는 것이 Generic 한 방법이다.
				// 특정 Ability 에 대한 의존성이 없으니깐.
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			ShowDamageNumber(EffectProperties, LocalIncomingDamage);
		}
	}
}

#pragma endregion

void UAuraAttributeSet::ShowDamageNumber(const FEffectProperties& EffectProperties, const float LocalIncomingDamage) const
{
	//if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(EffectProperties.SourceController))
	{
		PlayerController->ShowDamageNumber(LocalIncomingDamage, EffectProperties.TargetCharacter);
	}
}