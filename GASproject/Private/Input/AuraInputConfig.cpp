// Copyright NHE


#include "Input/AuraInputConfig.h"
#include "InputAction.h"


const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InInputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& AuraInputAction : AbilityInputActions)
	{
		if (AuraInputAction.InputAction && AuraInputAction.InputTag.MatchesTagExact(InInputTag))
		{
			return AuraInputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Ability Input Action for [Gameplay Tag : %s] is not found."), *GetNameSafe(this), *InInputTag.ToString());
	}

	return nullptr;
}
