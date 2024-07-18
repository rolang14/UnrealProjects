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

// Attack �������� �Ǵ�
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

	// Ÿ���� �̹� ���� ���¸� TARGET�� null�� �Ѵ�.
	bool IsTargetAlive = TargetStateInterface->CharacterAliveCheck();
	if (!IsTargetAlive)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		return false;
	}

	// �̷��� ���� ���Ϳ� ���� ������ �Ÿ��� ���� �� �ִ�.
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float PawnAIAttackRangeRadius = PawnAIInterface->GetAIAttackRange();

	bResult = (DistanceToTarget <= PawnAIAttackRangeRadius);
	UE_LOG(LogTemp, Log, TEXT("%s : Diatance(%f), PawnAttackRad(%f)"), bResult ? TEXT("true") : TEXT("false"), DistanceToTarget, PawnAIAttackRangeRadius);

	return bResult;
}
