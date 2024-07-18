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
	// 노드명과 인터벌 설정
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// Tick 마다 플레이어를 감지 시도
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	// 플레이어가 오버랩되는지 확인을 위한 오버랩 함수 호출을 위해 World 참조
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

	// NPC의 감지 거리를 가져오고 (Rad)
	float DetectRadius = PawnAIInterface->GetAIDetectRadius();
	// Overlap 하기 위해 OverlapResult 배열 선언
	TArray<FOverlapResult> OverlapResults;
	// Collision 하기 위해 Params 선언 (태그와 제외할 액터 추가)
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	// 그리고 오버랩하기위해 World에 Channel로 DetectRadius 범위의 스피어를 그려서 오버랩한다.
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
			// 오버랩 결과 중에서 Pawn 을 검사하여
			APawn* OverlappedPawn = Cast<APawn>(OverlapResult.GetActor());
			IABCharacterStateInterface* OverlappedPawnStateInterface = Cast<IABCharacterStateInterface>(OverlappedPawn);
			// Pawn이 존재하고 해당 Pawn의 컨트롤러가 플레이어 컨트롤러라면 (유저/*플레이어*/라면)
			// + 그리고 Alive 상태가 True 인 경우에만 체크를 한다.
			if (OverlappedPawn && OverlappedPawn->GetController()->IsPlayerController()
				&& OverlappedPawnStateInterface && OverlappedPawnStateInterface->CharacterAliveCheck())
			{
				// 타겟 (공격 목표) 를 OverlappedPawn 으로 설정한다.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, OverlappedPawn);
				// 디버그용 : NPC의 Detect 범위를 초록색으로 그린다.
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				// 마찬가지로 발각된 플레이어에게 포인트(점) 그리기
				DrawDebugPoint(World, OverlappedPawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				// 그리고 색적한 NPC와 발각된 플레이어 사이에 라인 그리기
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), OverlappedPawn->GetActorLocation(), FColor::Green, false, 0.27f);

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
