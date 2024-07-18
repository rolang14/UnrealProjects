// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AI/ABAIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* PawnAIInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (PawnAIInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// ��������Ʈ�� �Լ� ���ε�
	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			// InProgress �� Return �� ������, �� Task�� ����Ǿ����� �˷��ִ� �Լ�
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	// ��������Ʈ�� ����
	PawnAIInterface->SetAIAttackDelegate(OnAttackFinished);
	PawnAIInterface->AttackByAI();
	return EBTNodeResult::InProgress;
}
