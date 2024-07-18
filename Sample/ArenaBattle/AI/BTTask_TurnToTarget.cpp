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
	// �ٶ� ���� ��� : Ÿ�� ���� - ��� ����, �� �� ������ ���̰� Operand�� �ٶ���� �� ������ ��Ÿ����.
	FVector LookVector = TargetPawn->GetActorLocation() - AIPawn->GetActorLocation();
	// ���� �������� ���� (Z�� ȸ���� �� ���� ����)
	LookVector.Z = 0.0f;
	// X������ ȸ������ �� LookVector �� ������ �� �ִ� ȸ�� ����� �����.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// NPC ���� ���� �����̼���, RInterpTo(Ease Out (y = -ax^2-b �׷���)) �������.
	// �׸��� TargetRot ������ ���� DeltaSecond�� Turn Speed �ӵ��� ȸ����Ŵ
	AIPawn->SetActorRotation(FMath::RInterpTo(AIPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
