// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);


/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

private:
	// 마우스 Trace 위치를 얻고, Delegate 를 발생시킨다.
	virtual void Activate() override;

	// 서버로 TargetData 를 Replicate 한다.
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& InHandle, FGameplayTag InActivationTag);

public:
	// 마우스 타겟 데이터 Broadcast 용 Delegate
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature OnReceivedTargetData;

};
