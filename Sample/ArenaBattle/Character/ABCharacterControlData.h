// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABCharacterControlData();

	// ��Ʈ�ѷ��� Yaw ���� Pawn �����̼� ������ ����� ������
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// �� ������ CharacterMovement�� ���� ������
	// Movement�� �ڽ��� OrientRotation �� �ݿ��� ������ (ī�޶� ����� �������, �ڽ��� Orient Rotation �� ����ϴ���)  
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	// Controller Desired Rotation �� �ݿ��� ������ (��Ʈ�ѷ��� ��ǥ�ϴ� �������� Rotation �� ��ȭ��ų ������)
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	// ȸ�� �ӵ�
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	// InputMappingContext �� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Spring Arm �� ���� ������
	// Sprint Arm ���� (ī�޶� �Ÿ���� ���� ����)
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	// ?
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;
	
	// SpringArm�� PawnControlRotation �� ���� ������(?)
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	// ���� Pitch, Yaw, Roll �� ���� Rotation ���� ����� ������ 
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	// Collision �� ��·�� SprintArm �� Character ���̿� Collision �� �߻��ϸ� ī�޶� �����ٰ������� ���Ѱǵ�
	// test? �� ��... �� ���� �׷���.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;


private:

};
