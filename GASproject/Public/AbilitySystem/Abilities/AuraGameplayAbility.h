// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void UpdateWarpTarget(const FVector& TargetLocation);
	
public:
	// CDO에만 적용된다. 런타임중에는 Gameplay Ability Spec 을 사용할 것이기 때문에, 클래스 멤버를 사용하기가 어렵다.
	// 예를 들면, 스킬 조작 위치를 바꿔서 Left Click 이었던 것을 Right Click 으로 바꾼다던지, 1번과 2번의 스킬을 변경한다던지 등등을
	// 아래의 변수를 가지고 제어할 수 없다는 말이다.
	// 근데 어쨌든 처음 스킬을 얻었을 때 적용될 Input Tag 로는 사용할 수 있으니, 그런 목적으로 사용할 것이다.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	// 이 어빌리티가 부여할 대미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat AbilityDamage;
};
