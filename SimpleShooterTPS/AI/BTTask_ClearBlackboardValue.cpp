// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

// OwnerComp 는 참조이므로 반드시 null이 아니다. NodeMemory 는 ExecuteTask 를 위해 모든 블랙보드가 사용하는 싱글톤같은 메모리 주소가 존재하는데, 여기에서만 작동을 하게 하기 위함이다.
EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// 키 이름을 직접 쓰는 대신에 GetSelectedBlackboardKey 를 사용할 수 있다.
	//OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("LastKnownPlayerLocation"));
	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}

