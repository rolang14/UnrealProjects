// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AI/ABAIController.h"
#include "AI/ABAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"
#include "Interface/ABCharacterStateInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

// Attack 가능한지 판단
bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	IABCharacterAIInterface* PawnAIInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (PawnAIInterface == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
	{
		return false;
	}

	IABCharacterStateInterface* TargetStateInterface = Cast<IABCharacterStateInterface>(Target);
	if (TargetStateInterface == nullptr)
	{
		return false;
	}

	// 타겟이 이미 죽은 상태면 TARGET을 null로 한다.
	bool IsTargetAlive = TargetStateInterface->CharacterAliveCheck();
	if (!IsTargetAlive)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		return false;
	}

	// 이렇게 쉽게 액터와 액터 사이의 거리를 구할 수 있다.
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float PawnAIAttackRangeRadius = PawnAIInterface->GetAIAttackRange();

	bResult = (DistanceToTarget <= PawnAIAttackRangeRadius);
	UE_LOG(LogTemp, Log, TEXT("%s : Diatance(%f), PawnAttackRad(%f)"), bResult ? TEXT("true") : TEXT("false"), DistanceToTarget, PawnAIAttackRangeRadius);

	return bResult;
}
