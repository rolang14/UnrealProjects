// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DisappearWeapon.h"
#include "Character/RBCharacterBase.h"
#include "Weapon/RBWeaponTypeBase.h"
#include "Interface/RBWeaponFieldAttackInterface.h"

void UAnimNotify_DisappearWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}

	// 캐릭터의 무기 정보 가져오기
	ARBCharacterBase* CharacterBase = Cast<ARBCharacterBase>(MeshComp->GetOwner());
	if (CharacterBase == nullptr)
	{
		return;
	}

	// 해당 캐릭터 무기가 FieldAttackInterface 를 구현했다면 SummonEffect 재생
	if (IRBWeaponFieldAttackInterface* RBWeaponFieldAttackInterface = Cast<IRBWeaponFieldAttackInterface>(CharacterBase->GetCharacterWeapon()))
	{
		RBWeaponFieldAttackInterface->PerformWeaponDisappearEffect();
	}
}
