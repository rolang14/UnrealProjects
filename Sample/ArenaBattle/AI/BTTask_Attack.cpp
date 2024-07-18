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

	// 델리게이트에 함수 바인딩
	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			// InProgress 로 Return 한 다음에, 본 Task가 종료되었음을 알려주는 함수
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	// 델리게이트를 세팅
	PawnAIInterface->SetAIAttackDelegate(OnAttackFinished);
	PawnAIInterface->AttackByAI();
	return EBTNodeResult::InProgress;
}
