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
	// 상위 함수 호출하고 결과를 받아옴 (이전 노드의 결과를 받아오는 것)
	// BTTaskNode 에서 이 함수의 주석을 보면
	// starts this task, should return Succeeded, Failed or InProgress
	// 이렇게 적혀 있는데, 이는 Task를 실행하는 함수이며, 반드시 Succeeded, Failed, InProgress 셋 중 하나를 반환한다고 되어 있다.
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 여기서는 OwnerComp 가 지금 이 Task를 소유한 BehaviorTree Component 라고 되어있고
	// 여기서 BTree의 GetAIOwner를 실행함으로써, 시그니처 반환형을 봐도 알겠지만 AAIController 이다.
	// 즉, BTree를 AI Controller 가 소유하고 있으므로, 이 Controller를 가져와서
	// 지금 이 Controller 이 빙의된 폰을 아래와 같이 가져올 수 있음을 의미한다.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	// 만약 폰이 없다면 실패를 리턴.
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 그리고 이제 네비게이션 시스템을 가져와서, NavMesh 영역을 가져오고
	// 그 영역 내에서 랜덤한 위치를 선정하고자 하므로
	// 아래와 같이 그 폰이 속한 월드의 NavSystem 을 가져온다.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 이제 BlackBoard에 정의된 HomePos 값을 가져온다.
	FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	// OriginPos 로부터, 500 반경안에서, Navgation Mesh 에서 설정된 NavigableRadius 포인트를 랜덤으로 받아와서
	// NextPatrolPos 를 정한다.
	if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, 500.0f, NextPatrolPos))
	{
		// 정의가 되면, 다시 PatrolPos 를 이 위치로 정의하고 성공 표시를 한 뒤 종료한다.
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
