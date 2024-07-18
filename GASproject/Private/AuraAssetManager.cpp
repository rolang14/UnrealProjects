// Copyright NHE


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"


const UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Gameplay Tag 를 초기화한다.
	FAuraGameplayTags::InitializeNativeGameplayTags();
	// Target Data 를 사용하기 위해 아래의 함수 호출이 필요하다.
	UAbilitySystemGlobals::Get().InitGlobalData();
}
