// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// ������
	AABAIController();

	// ���� AI ����� ����
	void RunAI();
	void StopAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	// BlackBoard, Behavior Tree �ּ� ����� ����
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
