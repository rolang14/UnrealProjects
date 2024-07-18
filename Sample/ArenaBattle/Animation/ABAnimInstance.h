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
	// AnimInstance가 초기화 될때 한번 실행됨.
	virtual void NativeInitializeAnimation() override;
	// 매 프레임마다 실행됨.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 이 AnimInstance 를 소유하는 캐릭터 객체 (획득용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// 그 캐릭터의 Movement Component (획득용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 값 변수에는 블루프린트에서도 수정 가능하도록 EditAnywhere 으로 지정
	// 현재 캐릭터의 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// bool 값을 써도 되긴 하는데, 이게 모든 플랫폼에서 정확하게 동일한 사이즈를 갖는 것이 아니라서
	// 우리가 컨트롤하기에 좋지가 않다. 그래서 명확한 사이즈를 위해 언리얼에서는 integer 형으로 선언한다.
	// 이 때, 다른 Integer 형과 구분하기 위해서 앞에 b라는 boolean 접두사를 붙여서 사용한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// 현재 움직이고있는지 쉬고 있는지를 점검하기 위한 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	// 현재 떨어지고 있는지?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// 현재 점프 중인지?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// 점프중인지 아닌지를 점검하기 위한 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;
	
private:

};
