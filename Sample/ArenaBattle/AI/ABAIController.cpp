// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ABAI.h"

AABAIController::AABAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/ArenaBattle/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ArenaBattle/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}


}

void AABAIController::RunAI()
{
	// AI ������ ���ؼ��� ������κ��� ������ ������Ʈ�� �����;� ��.
	// AI Controller �� ������ Blackboard �� �����ͼ�
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	// �츮�� ����� ���� ������ ������ �ּ��� ������ ������Ʈ�� ���ٹڴ´�.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// �� ��Ʈ�ѷ��� ���� ���� (NPC) �� ��ġ�� ������ HOME Ű�� ������ (Vector ������) �����Ѵ�.
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// �����尡 �� �ʱ�ȭ�Ǹ� �̸� ������� BT�� �����Ѵ�.
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AABAIController::StopAI()
{
	// ���� �������� Brain Component �� BT���
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		// ������ �����Ѵ�. �ñ״�ó�� ���� ���ڰ� �⺻������ EBTStopMode::Safe ��, �����ϰ� ����ǵ��� ������ �Ǿ��ִ�.
		BTComponent->StopTree();
	}
}

// ���� ������ �Ǿ��� �� ȣ��
void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AI�� �����Ѵ�.
	RunAI();
}
