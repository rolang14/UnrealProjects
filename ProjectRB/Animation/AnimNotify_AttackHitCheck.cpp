// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/RBCharacterFieldAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// Field Attack 공격 판정에 대한 인터페이스 함수 호출
		if (IRBCharacterFieldAttackInterface* FieldAttackInterface = Cast<IRBCharacterFieldAttackInterface>(MeshComp->GetOwner()))
		{
			FieldAttackInterface->IsFieldAttackHit();
		}
	}
}
