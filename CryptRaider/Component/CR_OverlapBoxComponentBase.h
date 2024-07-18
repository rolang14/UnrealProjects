// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CR_OverlapBoxComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API UCR_OverlapBoxComponentBase : public UBoxComponent
{
	GENERATED_BODY()

public: 
	UCR_OverlapBoxComponentBase();

protected:
	AActor* FindOverlappedActorWithTag(const FName &InActorTag) const;
	AActor* FindOverlappedActorBurning() const;
};
