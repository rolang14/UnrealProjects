// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
// 캐릭터인지 확인하기 위해 ABCharacterBase 를 가져와서 캐스트해주고싶은건데
// 이거를 이렇게 사용하면 캐릭터의 종류가 늘어날때마다 이걸 하나하나 다 가져와야해서 범용적이지 않고 생산성이 떨어진다.
// 그러므로 인터페이스를 사용해야 한다.
// #include "Character/ABCharacterBase.h"
#include "Interface/ABAnimationAttackInterface.h"

// 보면 이제 Skeletal Mesh Component와 Animaion Sequence, AnimNOtifyEvent가 있다.
void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	// Mesh Component 가 캐릭터인지를 확인하고 싶음.
	// -> 인터페이스로 변경 후 어택을 체크하는 해당 인터페이스가 구현되어있는지를 체크하면 된다.
	if (MeshComp)
	{
		// 해당 인터페이스가 구현되어있다면 캐스팅이 될거고
		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		// NULL이 아닐 때 해당 인터페이스의 AttackHitCheck 를 호출하면 되는 것이다.
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
