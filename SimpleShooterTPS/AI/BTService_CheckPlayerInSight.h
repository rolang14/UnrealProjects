// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckPlayerInSight.generated.h"

/**
 * 플레이어가 시야에 있는지 계속해서 확인하고 플레이어의 위치를 업데이트하는 노드
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_CheckPlayerInSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayerInSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
