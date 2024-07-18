// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RBAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/RBCharacterBase.h"
#include "RBAnimInstance.h"

URBAnimInstance::URBAnimInstance()
{

}

void URBAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwnerCharacter)
	{
		CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void URBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = CharacterMovementComponent->Velocity.Size2D();

		ARBCharacterBase* RBCharacter = Cast<ARBCharacterBase>(OwnerCharacter);
		if (RBCharacter)
		{
			DesiredYaw = RBCharacter->DesiredYaw;
		}

		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = CharacterMovementComponent->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	}
}
