// Copyright NHE


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxHealth::UMMC_MaxHealth()
{
	// AuraAttributeSet 으로부터 Attribute를 가져 온다.
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	// 그냥 이렇게 static function 으로 가져오는 이유는, 어차피 GE가 적용될 때 계산이 되기 때문에 이미 Source 와 Target 이 누군지 알고 있다. 그래서 굳이 소유자가 필요 없는 것이다.
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// 스냅샷이란, 계산 순서와 관련이 있다. 지금같은 경우 GE가 적용되는 즉시 계산을 하므로 snapshot 이 필요 없지만
	// 만약 나중에 적용이 되고, 이전 값이 필요하다면 그때 사용할 수 있는 것이 Snapshot 이다. 이름이랑 똑같다.
	// 예를 들면 파이어볼을 발사할 때 발사 자체는 Spec이 생성되는것으로 볼 수 있지만, 실제로는 시간이 흐른 뒤 누군가 피격되어야 Modifier 를 적용할 것이다. 이럴 때 필요하다.
	// Snapshot = false 라면 GE가 생성될 때의 값을 획득하며, true 라면 그 반대가 된다.
	VigorDef.bSnapshot = false;

	// 캡쳐할 Attribute 설정을 완료하면, 계산을 위해 특정한 Array가 필요하다.
	// RelevantAttributesToCapture 에 Vigor 를 담으면 된다.
	// 이렇게 담아 놓으면, Modifier가 실행될 때 Effect가 적용되면 RelevantAttributesToCapture 에 값이 담긴다.
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gameplay Tag 를 여기서 얻는 방법
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Attribute 를 캡쳐하고 Magnitude 값을 얻는 방법
	// 얻어온 Tags 들을 여기에 세팅한다.
	// 이들은 Attribute를 캡쳐하기 위해 특정 함수에 던져야 하는 파라미터들이다.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 해서 아래에 캡쳐하고 싶은걸 던지면 된다.
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	// 음수를 방지하기 위해 일반적으로 사용하는 방법
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// 이제 Vigor 는 가져왔으니, CharacterLevel 을 가져와야 한다.
	// Context 에서 SourceObject 를 가져오면 GE를 만든놈을 알 수가 있는데, 지금의 상황에서는 바로 이게 캐릭터다.
	ICombatInterface* CombatInterface = CastChecked<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level = CombatInterface->GetCharacterLevel();

	// 이젠 갖고온걸로 알아서 수식을 정하면 된다.
	return 80.f + (2.5f * Vigor) + (10.f * Level);
}
