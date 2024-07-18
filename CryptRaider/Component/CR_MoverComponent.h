// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/EffectSoundInterface.h"
#include "Kismet/GameplayStatics.h"
#include "CR_MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UCR_MoverComponent : public UActorComponent, public IEffectSoundInterface
{
	GENERATED_BODY()

public:	
	UCR_MoverComponent();

	FORCEINLINE void SetReadyToMove(const bool InIsReadyToMove) { IsReadyToMove = InIsReadyToMove; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetOriginLocation(const FVector InOriginLocation) { OriginLocation = InOriginLocation; }
	FORCEINLINE void SetMoveTo(const FVector InMoveTo) { MoveTo = InMoveTo; }
	FORCEINLINE void SetMoveTime(const float InMoveTime) { MoveTime = InMoveTime; }

public:
	virtual void PlayEffectSound() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Move Action")
	TArray<TObjectPtr<USoundBase>> ActionSounds;

private:
	UPROPERTY(EditAnywhere, Category = "Move Action")
	FVector MoveTo;

	UPROPERTY(EditAnywhere, Category = "Move Action")
	float MoveTime;

	bool IsReadyToMove = false;
	bool IsPlayingSounds = false;

	AActor* OwnerActor;
	FVector OriginLocation;
};
