// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 여기서는 AnimInstance 가 초기화되므로 Owner 오브젝트에 대한 포인터 값을 넣어주도록 한다.
	// 생성자에서 하면 아직 액터가 안만들어졌을 가능성이 있음.
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		// 무브먼트 역시 마찬가지.
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 여기서는 원하는 값들을 가져온다.
	// 그리고 이건 매 프레임 호출되기 때문에 얘들은 이제 매 프레임마다 업데이트 되는게 된다.
	// 복잡한 연산을 절대 여기서 남발하면 안된다.
	if (Movement)
	{
		// Movement의 속도 값을 가져옴.
		Velocity = Movement->Velocity;
		// GroundSpeed 는 Z값을 제외한 X,Y크기만 (Size2D) 을 구해서 구할 수 있다.
		GroundSpeed = Velocity.Size2D();
		// 대기 상태인지의 판단은, GroundSpeed가 MovingThreshold(기준값) 보다 느리면 대기라고 판단함. 
		bIsIdle = GroundSpeed < MovingThreshold;
		// 떨어지고 있는지는 그냥 Movement 에서 가져오면 된다.
		bIsFalling = Movement->IsFalling();
		// 점프하고 있는지는 현재 떨어지고 있는 중이고, Z축 속도가 JumpingThreshold 속도보다 크면 점프 중이라고 판단한다.
		// 그리고, 만약 떨어지는 중이라면 아래로 낙하 중일테니 Z값이 음수여야 한다.
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);

	}
}
