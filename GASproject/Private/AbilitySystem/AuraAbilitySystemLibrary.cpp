// Copyright NHE


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Kismet/GameplayStatics.h"


UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// OverlayWidgetController 를 얻어오기 위해 다시 아래와 같이 파라미터들을 가져온다.

	// WorldContextObject 로부터 LocalPlayerController 을 가져온다.
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCP(PC, PS, ASC, AS);
			return AuraHUD->GetOverlayWidgetController(WCP);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WCP(PC, PS, ASC, AS);
			return AuraHUD->GetAttributeMenuWidgetController(WCP);
		}
	}

	return nullptr;
}

// Character Class Info 를 불러오고, 적용시키는 함수.
// 이를 위해서 호출하는 캐릭터의 Class 와 Level, AbilitySystemComponent 가 필요하다.
void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* InAbilitySystemComponent, const ECharacterClass InCharacterClass, const float InLevel)
{
	// Default Attribute 를 적용하는 Source 는 나 자신(Avatar Actor)이다.
	// Detail for MMCs
	const AActor* SourceActor = InAbilitySystemComponent->GetAvatarActor();

	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo* ClassDefaultInfo = &CharacterClassInfo->GetClassDefaultInfo(InCharacterClass);

	FGameplayEffectContextHandle PrimaryContextHandle = InAbilitySystemComponent->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle PrimarySpecHandle = InAbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo->PrimaryAttributes, InLevel, PrimaryContextHandle);
	InAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = InAbilitySystemComponent->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle SecondarySpecHandle = InAbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, InLevel, SecondaryContextHandle);
	InAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = InAbilitySystemComponent->MakeEffectContext();
	VitalContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle VitalSpecHandle = InAbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, InLevel, VitalContextHandle);
	InAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* InAbilitySystemComponent)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommomAbilities)
	{
		// Ability Spec 을 만든 다음
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		// 어빌리티를 부여한다. InputTag 가 Enemy 에는 필요없기때문에 기존에 부여하던 방식에서 태그 부분만 제외했다.
		// 태그 부분이 제외되면 사실상 AuraGameplayAbility 를 쓸 필요가 없기 때문에 일반 어빌리티로 만든다. (GameplayAbility)
		InAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

const UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(WorldContextObject);
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameModeBase);
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}
