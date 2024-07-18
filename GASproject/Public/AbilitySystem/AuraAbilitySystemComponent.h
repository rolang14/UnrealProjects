// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAssetTagsChanged, const FGameplayTagContainer& /* Asset Tags */);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Owner Actor, Avatar Actor 가 세팅된 이후 이 함수 호출을 통해 델리게이트를 바인딩한다.
	// 이렇게 하는 이유는, 캐릭터에 대한 ASC의 의존성을 없애기 위해서이다.
	void SetupAbilitySystemComponent();
	
#pragma region For Gameplay Effects

private:
	// GE가 적용될 때 호출될 델리게이트의 콜백 함수. OnGameplayEffectAppliedDelegateToSelf 델리게이트의 경우 서버에서만 호출되기 때문에 아래 함수를 RPC로 만들어, 클라이언트에서도 불리도록 한다.
	// Client : 해당 오브젝트를 소유한 클라이언트에 호출되도록 한다. Prefix 를 Client 로 사용하는 관행이 있으며, 이 RPC 함수를 구현할 때에는 선언했던 함수의 _implementation 이 반드시 Postfix로 붙어야 한다. 그 이유는 내부적으로 네트워킹 과정을 거친 뒤 Implementation 을 복사하기 때문이다.
	// Reliable : 해당 함수를 Reliable 상태로 만든다. 이는 Reliable 한 Function 이라는 뜻이며, 네트워크 상태와 관계 없이 반드시(?) Replicated 되어 목적지에 도착하는 것을 보장한다.
	UFUNCTION(Client, Reliable)
	void ClientGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

#pragma endregion

#pragma region For Gameplay Tags

// For Gameplay Tags Delegate
public:
	FOnEffectAssetTagsChanged OnEffectAssetTagsChanged;

#pragma endregion

#pragma region For Gameplay Abilities

public:
	// 캐릭터에게 어빌리티를 부여한다.
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses);

	void AbilityInputTagHold(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

#pragma endregion


};
