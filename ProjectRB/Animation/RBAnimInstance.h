// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RBAnimInstance.generated.h"

// 전방선언
class ACharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECTRB_API URBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URBAnimInstance();
	
protected:
	// AnimInstance 초기화 (BeginPlay 와 거의 동일함)
	virtual void NativeInitializeAnimation() override;
	// (Tick 과 거의 동일함)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	float DesiredYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	float MovingThreshold = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	float JumpingThreshold = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement")
	float NextMotionRatio = .1f;

private:
	// 이 Animation 을 소유하는 캐릭터
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	TObjectPtr<ACharacter> OwnerCharacter;

	// Character Movement 속성들을 이용하기 위함.
	UPROPERTY(VisibleDefaultsOnly, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
};
