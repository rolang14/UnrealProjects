// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SSAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

	// AI 생존 확인
	const bool IsDead() const;
	
protected:
	virtual void BeginPlay() override;
	
private:
	//UPROPERTY(EditAnywhere, Category = "Follow Player")
	//float AcceptanceRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Behavior")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};
