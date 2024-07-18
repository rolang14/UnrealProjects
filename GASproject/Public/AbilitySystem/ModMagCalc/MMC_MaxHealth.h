// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();

	// 모든 계산 결과는 float여야 한다.
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// Vigor Definition.
	// 캡쳐하고 싶은 Attribute 를 만들면 된다. 여기서는 MaxHealth 에 적용될 Vigor 를 캡쳐하고 싶으므로 이렇게..
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
