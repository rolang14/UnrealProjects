// Copyright NHE


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	// 타겟으로부터 ATtribute 를 가져온다.
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 파라미터를 정의하고
	FAggregatorEvaluateParameters EvalutateParameters;
	EvalutateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalutateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 캡쳐된 Attribute 를 가져온 다음
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvalutateParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	// 필요한 정보도 가져오고
	ICombatInterface* CombatInterface = CastChecked<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level = CombatInterface->GetCharacterLevel();

	// 그것들을 기반으로 계산한다.
	return 50.f + (3.f * Intelligence) + (5.f * Level);
}
