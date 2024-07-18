// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"


// Input Action 과 Input Tag 를 매핑하기 위한 구조체
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Tag를 통해 Ability Input Action 을 찾는다.
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InInputTag, bool bLogNotFound = false) const;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
