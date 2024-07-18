// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdatePlayerLocation.generated.h"

/**
 * 플레이어 위치를 업데이트 하기 위한 서비스 노드
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_UpdatePlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdatePlayerLocation();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
