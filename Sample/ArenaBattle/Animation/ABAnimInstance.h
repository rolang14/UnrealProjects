// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();

protected:
	// AnimInstance�� �ʱ�ȭ �ɶ� �ѹ� �����.
	virtual void NativeInitializeAnimation() override;
	// �� �����Ӹ��� �����.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// �� AnimInstance �� �����ϴ� ĳ���� ��ü (ȹ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// �� ĳ������ Movement Component (ȹ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// �� �������� �������Ʈ������ ���� �����ϵ��� EditAnywhere ���� ����
	// ���� ĳ������ �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// bool ���� �ᵵ �Ǳ� �ϴµ�, �̰� ��� �÷������� ��Ȯ�ϰ� ������ ����� ���� ���� �ƴ϶�
	// �츮�� ��Ʈ���ϱ⿡ ������ �ʴ�. �׷��� ��Ȯ�� ����� ���� �𸮾󿡼��� integer ������ �����Ѵ�.
	// �� ��, �ٸ� Integer ���� �����ϱ� ���ؼ� �տ� b��� boolean ���λ縦 �ٿ��� ����Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// ���� �����̰��ִ��� ���� �ִ����� �����ϱ� ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	// ���� �������� �ִ���?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// ���� ���� ������?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// ���������� �ƴ����� �����ϱ� ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;
	
private:

};
