// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Shoot.h"
#include "AIController.h"
#include "Character/SSCharacterBase.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (ASSCharacterBase* ShooterCharacter = Cast<ASSCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		ShooterCharacter->Fire();
		UE_LOG(LogTemp, Display, TEXT("AI Shot !!"));
		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Display, TEXT("AI Shot Failed !!"));
	return EBTNodeResult::Failed;
}
