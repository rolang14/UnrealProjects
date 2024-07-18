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
	// AI 실행을 위해서는 블랙보드로부터 블랙보드 컴포넌트를 가져와야 함.
	// AI Controller 에 설정된 Blackboard 를 가져와서
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	// 우리가 만들어 놓은 블랙보드 데이터 애셋을 블랙보드 컴포넌트에 갖다박는다.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 이 컨트롤러가 붙은 액터 (NPC) 의 위치를 가져와 HOME 키의 값으로 (Vector 값으로) 설정한다.
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 블랙보드가 잘 초기화되면 이를 기반으로 BT를 실행한다.
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AABAIController::StopAI()
{
	// 현재 실행중인 Brain Component 가 BT라면
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		// 실행을 중지한다. 시그니처를 보면 인자가 기본적으로 EBTStopMode::Safe 즉, 안전하게 종료되도록 설정이 되어있다.
		BTComponent->StopTree();
	}
}

// 폰에 부착이 되었을 때 호출
void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AI를 실행한다.
	RunAI();
}
