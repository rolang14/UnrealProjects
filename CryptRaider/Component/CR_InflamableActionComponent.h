// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CR_OverlapBoxComponentBase.h"
#include "CR_InflamableActionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartDissolve, UMaterialInstanceDynamic*, DissolveMaterial);

/**
 * 
 */
UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UCR_InflamableActionComponent : public UCR_OverlapBoxComponentBase
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	void DestroyWithFire(const float DeltaTime);

protected:
	UPROPERTY(BlueprintAssignable)
	FOnStartDissolve OnStartDissolve;

	UPROPERTY(EditAnywhere, Category = "Burn Material")
	TObjectPtr<UMaterialInstance> BurnMaterial;

private:
	AActor* FireActor;
	bool bIsOnFire = false;
};
