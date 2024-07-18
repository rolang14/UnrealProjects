// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RBCharacterControlData.generated.h"

// 카메라 줌 등에 사용할 Smooth 애니메이션 보강 타입
UENUM()
enum class SmoothType : uint8
{
	Default = 0,
	EaseOut,
	Lerp
};

/**
 * 인게임 X 개발용 캐릭터의 컨트롤 데이터 모음 (블루프린트용)
 */
UCLASS()
class PROJECTRB_API URBCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Spring Arm 에 적용되는 옵션들.
	// Pawn을 Control 하는 Rotation 을 카메라가 사용할 것인지에 대한 옵션.
	// 만약 이게 false 라면 카메라는 Pawn의 위치를 고수하려고 하므로 Pitch가 컨트롤러를 따라 움직이지 않는다.
	// 이 값이 true 라면 컨트롤로 입력된 Rotation 을 잘 따라갈 것이다.
	UPROPERTY(EditAnywhere, Category = "Spring Arm Control")
	uint8 bUsePawnControlRotation : 1;

	// 부모 컴포넌트의 Pitch 를 상속한다. 즉, 부모 Pitch 값을 알 수 있게 된다.
	UPROPERTY(EditAnywhere, Category = "Spring Arm Control")
	uint8 bInheritPitch : 1;

	// 부모 컴포넌트의 Yaw 를 상속한다. 즉, 부모 Yaw 값을 알 수 있게 된다.
	UPROPERTY(EditAnywhere, Category = "Spring Arm Control")
	uint8 bInheritYaw : 1;

	// 부모 컴포넌트의 Roll 를 상속한다. 즉, 부모 Roll 값을 알 수 있게 된다.
	UPROPERTY(EditAnywhere, Category = "Spring Arm Control")
	uint8 bInheritRoll : 1;

	// 카메라 스프링 길이
	UPROPERTY(EditAnywhere, Category = "Spring Arm Control")
	float TargetArmLength;

	// 부드러운 줌 강도
	UPROPERTY(EditAnywhere, Category = "Spring Arm Control")
	float SmoothZoomSpeed;

	// 줌 속도
	UPROPERTY(EditAnywhere, Category = "Character Camera Control")
	float ZoomSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Camera Control")
	SmoothType SmoothType;

	// 스프링암 소켓 오프셋
	UPROPERTY(EditAnywhere, Category = "Character Camera Control")
	FVector SpringArmSocketOffset;

	// Character Pawn 에 적용되는 옵션들.
	// Character Pawn 이 Controller의 Yaw Rotation 을 따라가도록 한다.
	// 즉, 이 옵션을 활성화하면 카메라의 Yaw 방향 회전에 따라 가만히 서있더라도 Pawn 또한 같이 회전한다.
	UPROPERTY(EditAnywhere, Category = "Character Pawn Control")
	uint8 bUseControllerRotationYaw : 1;

	// CharacterMovement 에 적용되는 옵션.
	// 캐릭터를 Move 하는 방향대로 회전시킬지를 결정하는 옵션임.
	// 정확하게는 캐릭터가 나아가려는 가속도의 방향와 동일한 방향으로 회전한다.
	UPROPERTY(EditAnywhere, Category = "Character Movement Control")
	uint8 bOrientRotationToMovement : 1;

	// 캐릭터가 나아갈 때, Controller 가 원하는 방향으로 회전하도록 한다.
	UPROPERTY(EditAnywhere, Category = "Character Movement Control")
	uint8 bUseControllerDesiredRotation : 1;

	// CharacterMovement 에 적용되는 옵션.
	// 캐릭터가 회전하는 경우 그 속도를 의미함.
	UPROPERTY(EditAnywhere, Category = "Character Movement Control")
	FRotator RotationRate;

	// 뛰는 속도
	UPROPERTY(EditAnywhere, Category = "Character Movement Control")
	float MaxWalkSpeed;

	// 걷는 속도
	UPROPERTY(EditAnywhere, Category = "Character Movement Control")
	float MinWalkSpeed;

	// Character Field Attack 에 적용되는 옵션.
	// Field Attack 모션이 시작되고 이 시간 이후에는 움직일 수 있게 된다.
	UPROPERTY(EditAnywhere, Category = "Character Field Attack Control")
	float FieldAttackMovableTime;
};