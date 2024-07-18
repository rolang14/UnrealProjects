// Copyright NHE


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"


void UAuraAbilitySystemComponent::SetupAbilitySystemComponent()
{
	// Instant, Duration GE가 적용되는 경우 콜백
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientGameplayEffectApplied);
}

void UAuraAbilitySystemComponent::ClientGameplayEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);
	OnEffectAssetTagsChanged.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses)
{
	// 어빌리티 적용
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilityClasses)
	{
		// Ability Spec 을 만든 다음
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		// 초기 Ability Tag 를 설정한다.
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// Dynamic Ability Tags 는 런타임에 동적으로 추가되거나 제거되도록 설계되어있다.
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHold(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 활성화가 "가능한" 어빌리티 중, 동일한 태그를 갖고 있는 어빌리티를 활성화한다.
	for (FGameplayAbilitySpec& GASpec : GetActivatableAbilities())
	{
		if (GASpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// Ability Input 이 Pressed 되었다고 표시한다.
			AbilitySpecInputPressed(GASpec);
			// 어빌리티가 활성화되지 않은 놈만
			if (!GASpec.IsActive())
			{
				// 활성화. Try는 외부 요인으로 인해 Ability를 활성화하지 못하는 경우, 활성화 시키지 않기 위해서다.
				TryActivateAbility(GASpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 활성화가 "가능한" 어빌리티 중, 동일한 태그를 갖고 있는 어빌리티를 활성화한다.
	for (FGameplayAbilitySpec& GASpec : GetActivatableAbilities())
	{
		if (GASpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// Ability Input 이 Pressed 되었다고 표시한다.
			AbilitySpecInputReleased(GASpec);
		}
	}
}
