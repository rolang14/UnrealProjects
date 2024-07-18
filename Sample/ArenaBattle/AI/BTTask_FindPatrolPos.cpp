// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "AI/ABAI.h"
#include "AI/ABAIController.h"
#include "NavigationSystem.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ���� �Լ� ȣ���ϰ� ����� �޾ƿ� (���� ����� ����� �޾ƿ��� ��)
	// BTTaskNode ���� �� �Լ��� �ּ��� ����
	// starts this task, should return Succeeded, Failed or InProgress
	// �̷��� ���� �ִµ�, �̴� Task�� �����ϴ� �Լ��̸�, �ݵ�� Succeeded, Failed, InProgress �� �� �ϳ��� ��ȯ�Ѵٰ� �Ǿ� �ִ�.
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ���⼭�� OwnerComp �� ���� �� Task�� ������ BehaviorTree Component ��� �Ǿ��ְ�
	// ���⼭ BTree�� GetAIOwner�� ���������ν�, �ñ״�ó ��ȯ���� ���� �˰����� AAIController �̴�.
	// ��, BTree�� AI Controller �� �����ϰ� �����Ƿ�, �� Controller�� �����ͼ�
	// ���� �� Controller �� ���ǵ� ���� �Ʒ��� ���� ������ �� ������ �ǹ��Ѵ�.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	// ���� ���� ���ٸ� ���и� ����.
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// �׸��� ���� �׺���̼� �ý����� �����ͼ�, NavMesh ������ ��������
	// �� ���� ������ ������ ��ġ�� �����ϰ��� �ϹǷ�
	// �Ʒ��� ���� �� ���� ���� ������ NavSystem �� �����´�.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// ���� BlackBoard�� ���ǵ� HomePos ���� �����´�.
	FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	// OriginPos �κ���, 500 �ݰ�ȿ���, Navgation Mesh ���� ������ NavigableRadius ����Ʈ�� �������� �޾ƿͼ�
	// NextPatrolPos �� ���Ѵ�.
	if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, 500.0f, NextPatrolPos))
	{
		// ���ǰ� �Ǹ�, �ٽ� PatrolPos �� �� ��ġ�� �����ϰ� ���� ǥ�ø� �� �� �����Ѵ�.
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
