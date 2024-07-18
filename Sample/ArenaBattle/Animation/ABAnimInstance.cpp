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

	// ���⼭�� AnimInstance �� �ʱ�ȭ�ǹǷ� Owner ������Ʈ�� ���� ������ ���� �־��ֵ��� �Ѵ�.
	// �����ڿ��� �ϸ� ���� ���Ͱ� �ȸ�������� ���ɼ��� ����.
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		// �����Ʈ ���� ��������.
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ���⼭�� ���ϴ� ������ �����´�.
	// �׸��� �̰� �� ������ ȣ��Ǳ� ������ ����� ���� �� �����Ӹ��� ������Ʈ �Ǵ°� �ȴ�.
	// ������ ������ ���� ���⼭ �����ϸ� �ȵȴ�.
	if (Movement)
	{
		// Movement�� �ӵ� ���� ������.
		Velocity = Movement->Velocity;
		// GroundSpeed �� Z���� ������ X,Yũ�⸸ (Size2D) �� ���ؼ� ���� �� �ִ�.
		GroundSpeed = Velocity.Size2D();
		// ��� ���������� �Ǵ���, GroundSpeed�� MovingThreshold(���ذ�) ���� ������ ����� �Ǵ���. 
		bIsIdle = GroundSpeed < MovingThreshold;
		// �������� �ִ����� �׳� Movement ���� �������� �ȴ�.
		bIsFalling = Movement->IsFalling();
		// �����ϰ� �ִ����� ���� �������� �ִ� ���̰�, Z�� �ӵ��� JumpingThreshold �ӵ����� ũ�� ���� ���̶�� �Ǵ��Ѵ�.
		// �׸���, ���� �������� ���̶�� �Ʒ��� ���� �����״� Z���� �������� �Ѵ�.
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);

	}
}
