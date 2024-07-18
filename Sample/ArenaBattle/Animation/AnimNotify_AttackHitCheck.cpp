// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
// ĳ�������� Ȯ���ϱ� ���� ABCharacterBase �� �����ͼ� ĳ��Ʈ���ְ�����ǵ�
// �̰Ÿ� �̷��� ����ϸ� ĳ������ ������ �þ������ �̰� �ϳ��ϳ� �� �����;��ؼ� ���������� �ʰ� ���꼺�� ��������.
// �׷��Ƿ� �������̽��� ����ؾ� �Ѵ�.
// #include "Character/ABCharacterBase.h"
#include "Interface/ABAnimationAttackInterface.h"

// ���� ���� Skeletal Mesh Component�� Animaion Sequence, AnimNOtifyEvent�� �ִ�.
void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	// Mesh Component �� ĳ���������� Ȯ���ϰ� ����.
	// -> �������̽��� ���� �� ������ üũ�ϴ� �ش� �������̽��� �����Ǿ��ִ����� üũ�ϸ� �ȴ�.
	if (MeshComp)
	{
		// �ش� �������̽��� �����Ǿ��ִٸ� ĳ������ �ɰŰ�
		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		// NULL�� �ƴ� �� �ش� �������̽��� AttackHitCheck �� ȣ���ϸ� �Ǵ� ���̴�.
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
