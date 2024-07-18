// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"



/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	// Template Function 으로 만들고 싶기 때문에 전방선언이 아니라 헤더를 포함한다. 왜냐면 헤더에 구현이 되니깐.
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HoldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HoldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc)
{
	check(InputConfig);
	for (const FAuraInputAction& AuraInputAction : InputConfig->AbilityInputActions)
	{
		if (AuraInputAction.InputAction && AuraInputAction.InputTag.IsValid())
		{
			if (PressedFunc) BindAction(AuraInputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, AuraInputAction.InputTag);
			if (ReleasedFunc) BindAction(AuraInputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AuraInputAction.InputTag);
			// Triggered 트리거는 누르고 있으면 계속해서 프레임마다 호출된다.
			if (HoldFunc) BindAction(AuraInputAction.InputAction, ETriggerEvent::Triggered, Object, HoldFunc, AuraInputAction.InputTag);
		}
	}
}
