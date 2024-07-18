// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditInstanceOnly, Category = "Move Option", Meta = (MakeEditWidget = "true"))
	FVector TargetLocationOffset;

	UPROPERTY(EditInstanceOnly, Category = "Move Option")
	float MoveSpeed;

	// Number Of Trigger 수 이상의 트리거가 활성화되면, 이 플랫폼을 움직인다.
	void AddActivatedTrigger();
	void RemoveActivatedTrigger();

private:
	// 이 플랫폼을 움직이기 위해 활성화 시켜야 하는 트리거의 수
	UPROPERTY(EditAnywhere)
	int NumberOfTrigger;

	int ActivatedTrigger;

	FVector GlobalStartLocation;
	FVector GlobalTargetLocation;
};
