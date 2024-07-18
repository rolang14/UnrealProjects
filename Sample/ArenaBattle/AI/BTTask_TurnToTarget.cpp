// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AI/ABAIController.h"
#include "AI/ABAI.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawnInterface = Cast<IABCharacterAIInterface>(AIPawn);
	if (AIPawnInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	float TurnSpeed = AIPawnInterface->GetAITurnSpeed();
	// 바라볼 방향 계산 : 타겟 벡터 - 대상 벡터, 즉 두 벡터의 차이가 Operand이 바라봐야 할 방향을 나타낸다.
	FVector LookVector = TargetPawn->GetActorLocation() - AIPawn->GetActorLocation();
	// 수평 방향으로 제한 (Z축 회전을 걍 없앰 ㅇㅇ)
	LookVector.Z = 0.0f;
	// X축으로 회전했을 때 LookVector 에 도달할 수 있는 회전 행렬을 계산함.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// NPC 폰의 액터 로테이션을, RInterpTo(Ease Out (y = -ax^2-b 그래프)) 방식으로.
	// 그리고 TargetRot 방향을 향해 DeltaSecond당 Turn Speed 속도로 회전시킴
	AIPawn->SetActorRotation(FMath::RInterpTo(AIPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
