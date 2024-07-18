// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_InterruptMotion.h"
#include "AnimNotify_InterruptMotion.h"
#include "Interface/RBCharacterFieldAttackInterface.h"

void UAnimNotify_InterruptMotion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (IRBCharacterFieldAttackInterface* FieldAttackInterface = Cast<IRBCharacterFieldAttackInterface>(MeshComp->GetOwner()))
		{
			FieldAttackInterface->InterruptFieldAttack();
		}
	}
}
