// Copyright NHE


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"

#include "AbilitySystemComponent.h"


/**
* 블루프린트나 다른곳에서 사용할 것이 아니므로 F 접두사도 안붙이고 USTRUCT 도 안함.
* 또, MMC에서 했던 FGameplayEffectAttributeCaptureDefinition 와, Attribute Capture 에 대한 부분을 구조체로 정의하여 사용하는 테크닉은 이와 같다.
* Statics 를 붙이는 이유는, 이 구조체를 static 으로 사용할 것이기 때문이다.
* 이 구조체 타입의 static variable instance 를 만들어서, 한번 만들고 난 이후에는 계속해서 동일한 객체에 접근하고 싶은 것이다.
*/

struct AuraDamageStatics
{
	// Target
	// Armor (Attribute Name) 라는 이름의 Attribute Capture Def 를 만들어 주는 매크로이다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	// 이 확률에 따라 Damage 가 1/2 로 적용된다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	// 이 스탯에 따라 크리티컬 확률을 줄인다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	// Source
	// Armor Penetration 은 Armor 을 일정 부분 무시한다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	// 이 확률에 따라 크리티컬 대미지가 적용된다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	// 이 스탯에 따라 크리티컬 대미지가 증가한다.
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	AuraDamageStatics()
	{
		// Target
		/**
		* 해당 클래스 내에 Armor(Attribute) 가 존재하는지 확인 후, 실제로 생성한다.
		* 그래서 아래의 매크로는 여기서 생성자 내부에 존재해야 한다.
		* 앞에서부터 해당 어트리뷰트가 정의된 클래스 (여기서는 UAuraAttributeSet), Attribute 이름,
		* 그리고 EGameplayEffectAttributeCaptureSource 가 들어가는데, 그냥 열거형 이름만 봐도 이 Attribute 를 어떤 놈에게서 캡쳐할 것인지를 의미한다. Source 혹은 Target 밖에 없는데,
		* 대미지 적용을 위해 Armor 를 갖고 온다면, 공격자가 아니라 피격자의 Armor 를 갖고와야 하는 것이 당연하다. 따라서 Target 을 넣는다.
		* 마지막 B는 Boolean 으로써 Snapshot 을 할것인지 아닌지를 의미한다. 궁금하면 매크로 정의를 뒤져보자.
		* 어쨌든, 그렇기 때문에 우리는 이 구조체가 이 클래스 내에서 쭉 사용되길 원하며 다른 곳에서 사용할 때마다 새로 만들기를 원하지 않는다.
		*/
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		// Source
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false)
	}
};

// 따라서 위의 구조체를 static 하게 사용하기 위해 아래와 같은 테크닉을 사용할 수 있다.
static const AuraDamageStatics& DamageStatics()
{
	/**
	* 이렇게 함수 내에 static 변수를 만들면, 이 함수를 부를때마다 새롭게 메모리에 할당하는 것이 아니라 Data 영역에 저장되게 된다.
	* 따라서 이 함수를 호출하면, 항상 동일한 AuraDamageStatics 인스턴스를 갖는다.
	*/
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

/**
* 생성자에서는 이제 위에서 정의한 AuraDamageStatics 를 특정 Attribute 캡쳐를 위해 사용할 것임을 알려야 한다.
* 그냥 저걸 사용해서 캡쳐하라는 말이다.
*/
UExecCalc_Damage::UExecCalc_Damage()
{
	/** RelevantAttributesToCapture 라는 배열에 추가함으로써 이를 소유한 Gameplay Effect 가 만들어지고, Modifier 계산을 위해 이 클래스가 인스턴싱될때
	* RelevantAttributesToCapture 에 추가되어있는 Attribute 들을 캡쳐할 것임을 지정할 수 있다. 그러므로 생성자 시점에서 호출해줘야하며
	* 이렇게 하면 생성자 코드가 굉장히 간단해진다.
	*/

	// Target
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	// Source
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

// 대미지 계산을 위해 Source, Target 의 Attribute 를 가져오는 등 많은 정보를 핸들링 해야 함.
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#pragma region Setting up

	// ExecutionParams 에서는 아래와 같이 GameplayEffect 와 관련된 굉장히 많은 정보를 가져올 수 있다.
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	/**
	* 구조체 이름이 어려워 보이지만 별게 없다.
	* Eval 은 Linux 부터 시작되어 온 실행자의 의미가 있다. 즉, Aggregator 를 실행하기 위해 필요한 Parameter 를 정의한다는 것.
	* Aggregator 는 iterator 를 실행하는 놈인데, 어떠한 정보를 모아서 처리한다는 의미가 강하다.
	* 그러니까 그냥 이건 Parmater 를 수집해서 뭔갈 실행하기 위해 필요한 놈이란거다.
	*/
	FAggregatorEvaluateParameters EvaluateParam;
	// 그러므로 아래와 같이 필요한 정보를 넣어서 AttemptCalculateCapturedAttributeMagnitude 를 실행하는 것이다.
	EvaluateParam.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParam.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

#pragma endregion

	// ExecCalc 에서는 아래와 같이 생성자 호출 시점에 RelevantAttributesToCapture 를 통해 캡쳐된 Attribute 를 불러올 수 있다.
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParam, TargetArmorValue);
	// ExecCalc 에서는 PreAttributeChanged 가 수행되지 않으므로, Clamping 을 ExecCalc 가 실행될 떄 다시 해줘야 한다. 보통 Attribute 값의 경우 음수는 나오지 않으므로 아래와 같이 조정할 수 있다.
	//TargetArmorValue = FMath::Max<float>(0.f, TargetArmorValue);

	/** 
	* 아래와 같이 특정 Attribute 에 대한 Modifier 를 설정할 수 있다.
	* 첫번째 생성자 인자로 const FGameplayAttribute& 를 받도록 되어 있는데 FProperty* 로 컴파일이 문제 없는 이유는
	* 실제로 GameplayAttribute 의 생성자 중에 FProperty 를 받는 함수가 있기 때문인데, 결론적으로 FProperty 를 래핑한다고 볼 수 있다.
	* FProperty 는 Unreal Reflection 에서 사용하는 기본 클래스이므로 이런 래핑이 존재한다. 즉, Modify 편하게 하고 싶으면 매크로.. 사용해야겠지..?
	* 궁금하면 FGameplayAttribute 생성자를 찾아보자.
	*/
	//const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, TargetArmorValue);

#pragma region Calculate Damage

	/*
	* Set by Caller Magnitude 를 이렇게 Exec Calc 에서 호출하고 결과를 확인할 수 있다.
	* 실제로 Set by Caller Magnitude 가 설정되는것은 Projectile Spell 코드를 확인해 보면 알겠지만 Projectile 이 소환되는 순간이다.
	* ExecCalc 는 실제로 Gameplay Effect 가 Apply 될 때 실행되므로, 반드시 Set by Caller Magnitude 가 설정되어 있으며, 가져올 수 있다.
	*/
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	/*
	* 캡쳐하기 전, 필요한 데이터를 불러온다. (AbilityLibrary -> GameMode)
	* WorldContextObject 는 그냥 아무거나 오브젝트를 넣으면 되어서 AActor 를 넣은것뿐이다.
	*/
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	const UCurveTable* DamageCalcCoefficients = CharacterClassInfo->DamageCalculationCoefficients;

#pragma region 1. Apply Armor and Armor Penetration

	/**
	* Capture Armor on Target
	* This defends some amounts of damage.
	* 1 is 1% Reduce Damage
	*/
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParam, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	/**
	* Capture ArmorPenetration on Source
	* This ignores a percentage of Target Armor and it can't be over 100.
	* 1 is ignores 1% of Armor.
	*/
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParam, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Clamp<float>(SourceArmorPenetration, 0.f, 100.f);

	// Get SourceArmorPen Coefficient from CurveTable
	float SourceArmorPenCoefficient = DamageCalcCoefficients->FindCurve(FName("ArmorPenetration"), FString())->Eval(SourceCombatInterface->GetCharacterLevel());
	// this means that 1 point of ArmorPenetration would ignores SourceArmorPenCoefficient(%) of Armor, in other way, 1/SourceArmorPenCoefficient points of ArmorPenetration would ignores 1% of Armor
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * SourceArmorPenCoefficient) / 100.f;

	// Get TargetArmor Coefficient from CurveTable
	float TargetEffectiveArmorCoefficient = DamageCalcCoefficients->FindCurve(FName("EffectiveArmor"), FString())->Eval(TargetCombatInterface->GetCharacterLevel());
	// this means that 1/TargetArmorCoefficient points of Armor would reduce damage by 1%
	// Armor Applied Execution Damage
	Damage *= (100 - EffectiveArmor * TargetEffectiveArmorCoefficient) / 100.f;

#pragma endregion

#pragma region 2. Apply Critical Hit

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParam, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParam, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	// Calculate Critical Hit Chance
	const float CriticalHitResistanceCoefficient = DamageCalcCoefficients->FindCurve(FName("CriticalHitResistance"), FString())->Eval(TargetCombatInterface->GetCharacterLevel());
	const float EffectiveCriticalHitChance = (SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient);
	const bool bIsCritical = FMath::RandRange(1, 100) <= EffectiveCriticalHitChance;
	if (bIsCritical)
	{
		// if Critical Hit, Calculate Critical Damage
		float SourceCriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParam, SourceCriticalHitDamage);
		SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);
		Damage *= (200 + SourceCriticalHitDamage) / 100.f;
	}

#pragma endregion

#pragma region 3. Apply Block Chance

	/**
	* Capture BlockChance on Target, and determine if there was s successful Block
	* If Block was successful, take half damage.
	* 1 is 1% Block Chance.
	* And If Critical Hit, Block will not be activated.
	*/
	else
	{
		float TargetBlockChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParam, TargetBlockChance);
		TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

		// Block 되었는지 확률로 판단
		const bool bIsBlocked = FMath::RandRange(1, 100) <= TargetBlockChance;
		Damage = bIsBlocked ? Damage * 0.5f : Damage;
	}

#pragma endregion

#pragma region 4. Modify IncomingDamage Meta Attribute

	GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Blue, FString::Printf(TEXT("Damage : %f, TargetArmor : %f, ActualArmor : %f, SourceArmorPenetration : %f, SourceCriticalHitChance : %f, TargetCriticalHitResistance : %f, ActualCriticalHitChance : %f"),
		Damage, TargetArmor, EffectiveArmor, SourceArmorPenetration, SourceCriticalHitChance, TargetCriticalHitResistance, EffectiveCriticalHitChance));

	// 아래와 같이 적용하면 Damage Meta Attribute 를 Add 할 수 있게 된다.
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);

	/**
	* Execute 함수에는 별도의 반환형이 없고, 매개 변수를 보면 결과를 OutExecutionOutput 에 수정하도록 되어있다.
	* 즉 위에서 Modify 한 결과를 넣을 수 있다는 것.
	*/
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

#pragma endregion

#pragma endregion


}
