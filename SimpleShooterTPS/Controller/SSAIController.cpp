// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SSAIController.h"
#include "Character/SSCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASSAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	RunBehaviorTree(BehaviorTree);

	// 플레이어 추적 후에 원래 자리로 돌아오기 위해서 현재 이 캐릭터의 시작 위치를 저장
	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
}

void ASSAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (PlayerPawn == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("No Player Pawn In this World."));
	//	return;
	//}

	//// AI 의 시야에서 PlayPawn 까지의 라인이 그려진다면
	//if (LineOfSightTo(PlayerPawn))
	//{
	//	//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	//	//GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	//}
	//// 시야를 벗어나면
	//else
	//{
	//	//GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	//}

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (PlayerPawn == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("No Player Pawn In this World."));
	//	return;
	//}

	//// AI 의 시야에서 PlayPawn 까지의 라인이 그려진다면
	//if (LineOfSightTo(PlayerPawn))
	//{
	//	// 플레이어를 추적 (플레이어에게 포커스)
	//	SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	//	// 플레이어 위치로 이동 (반경)
	//	MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//// 시야를 벗어나면
	//else
	//{
	//	ClearFocus(EAIFocusPriority::Gameplay);
	//	StopMovement();
	//}
}

// AI 생존 확인용 함수
const bool ASSAIController::IsDead() const
{
	if (ASSCharacterBase* AICharacter = Cast<ASSCharacterBase>(GetPawn()))
	{
		// 해당 캐릭터의 상태 확인
		return AICharacter->GetDead();
	}

	// CharacterBase 에서 모든 캐릭터는 사망 시 Controller를 분리하게 되어있으므로, 소유한 Pawn이 없다면 그 AI는 죽은 상태임
	return true;
}
