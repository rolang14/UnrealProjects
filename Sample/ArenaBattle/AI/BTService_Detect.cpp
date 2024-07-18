// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AI/ABAIController.h"
#include "AI/ABAI.h"
#include "Interface/ABCharacterAIInterface.h"
#include "Interface/ABCharacterStateInterface.h"
#include "Physics/ABCollision.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	// ����� ���͹� ����
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// Tick ���� �÷��̾ ���� �õ�
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	// �÷��̾ �������Ǵ��� Ȯ���� ���� ������ �Լ� ȣ���� ���� World ����
	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	IABCharacterAIInterface* PawnAIInterface = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (PawnAIInterface == nullptr)
	{
		return;
	}

	// NPC�� ���� �Ÿ��� �������� (Rad)
	float DetectRadius = PawnAIInterface->GetAIDetectRadius();
	// Overlap �ϱ� ���� OverlapResult �迭 ����
	TArray<FOverlapResult> OverlapResults;
	// Collision �ϱ� ���� Params ���� (�±׿� ������ ���� �߰�)
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	// �׸��� �������ϱ����� World�� Channel�� DetectRadius ������ ���Ǿ �׷��� �������Ѵ�.
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			// ������ ��� �߿��� Pawn �� �˻��Ͽ�
			APawn* OverlappedPawn = Cast<APawn>(OverlapResult.GetActor());
			IABCharacterStateInterface* OverlappedPawnStateInterface = Cast<IABCharacterStateInterface>(OverlappedPawn);
			// Pawn�� �����ϰ� �ش� Pawn�� ��Ʈ�ѷ��� �÷��̾� ��Ʈ�ѷ���� (����/*�÷��̾�*/���)
			// + �׸��� Alive ���°� True �� ��쿡�� üũ�� �Ѵ�.
			if (OverlappedPawn && OverlappedPawn->GetController()->IsPlayerController()
				&& OverlappedPawnStateInterface && OverlappedPawnStateInterface->CharacterAliveCheck())
			{
				// Ÿ�� (���� ��ǥ) �� OverlappedPawn ���� �����Ѵ�.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, OverlappedPawn);
				// ����׿� : NPC�� Detect ������ �ʷϻ����� �׸���.
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				// ���������� �߰��� �÷��̾�� ����Ʈ(��) �׸���
				DrawDebugPoint(World, OverlappedPawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				// �׸��� ������ NPC�� �߰��� �÷��̾� ���̿� ���� �׸���
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), OverlappedPawn->GetActorLocation(), FColor::Green, false, 0.27f);

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
