// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CR_OverlapBoxComponentBase.h"
#include "CR_FireEmitter.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UCR_FireEmitter : public UCR_OverlapBoxComponentBase
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
private:
	void SetFireOn(AActor* InflamableActor);

protected:
	UPROPERTY(EditAnywhere, Category = "Inflamable Target")
	FName InflamableTag;

	UPROPERTY(EditAnywhere, Category = "Burn Sound")
	TObjectPtr<USoundBase> BurnSound;

private:
	bool bIsBurnedOnce = false;
};
