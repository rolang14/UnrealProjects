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

	// 컨트롤러의 Yaw 값을 Pawn 로테이션 값으로 사용할 것인지
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// 그 다음은 CharacterMovement에 대한 설정값
	// Movement가 자신의 OrientRotation 을 반영할 것인지 (카메라 방향과 관계없이, 자신의 Orient Rotation 을 사용하는지)  
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	// Controller Desired Rotation 을 반영할 것인지 (컨트롤러가 목표하는 방향으로 Rotation 을 변화시킬 것인지)
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	// 회전 속도
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	// InputMappingContext 도 국밥이지.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Spring Arm 에 대한 설정값
	// Sprint Arm 길이 (카메라 거리라고 보면 편함)
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	// ?
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;
	
	// SpringArm이 PawnControlRotation 을 따라갈 것인지(?)
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	// 각각 Pitch, Yaw, Roll 에 대한 Rotation 값을 상속할 것인지 
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	// Collision 는 어쨌든 SprintArm 과 Character 사이에 Collision 이 발생하면 카메라를 땡겨줄것인지에 대한건데
	// test? 는 음... 머 대충 그렇다.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;


private:

};
