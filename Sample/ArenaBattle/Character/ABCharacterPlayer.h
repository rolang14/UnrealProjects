// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Interface/ABCharacterHUDInterface.h"
// EnhancedInput 모듈을 추가
#include "InputActionValue.h"
#include "ABCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase, public IABCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

// Character Control Section
protected:
	// V키를 눌러 입력 모드를 바꿈
	void ChangeCharacterControl();
	// 입력 모드 바뀐걸로 캐릭터 컨트롤을 Set
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	// CharacterBase 에 있으므로 반드시 override 해야한다.
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;

protected:
	// 입력 매핑 컨텍스트 할당용 (아래쪽의 DefaultMappingContext)
	virtual void BeginPlay() override;
	// 사망 설정
	virtual void SetDead() override;

public:
	// 플레이어 Input 컴포넌트를 셋업하는데, 여기서는 UInputAction (입력 액션)과 Move, Look 등의 함수를 매핑시켜주는 역할을 해야 한다.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

// For Camera
protected:
	// 스프링 암 : 카메라를 지탱해주는 지지대 역할을 하는 컴포넌트
	// 아래 Meta 같은 경우에는 Private로 선언된 언리얼 오브젝트 객체라도 BP에서 접근 가능하도록 만들어 준다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// 이거는 실제 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// For Input Section
protected:
	// 아래 3개는 우리가 원하는 액션을 지정하기 위함 (기본적으로 3인칭 템플릿에서 있는 것들이라 그냥 그정도만 따라서 만듦)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	// Shoulder, Quarter 로 바꾸면서 이제 얘들은 필요가 없다.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> MoveAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> LookAction;

	//// InputMappingContext 정의
	//// 에셋들의 경우에는 다른 에셋으로 변환하기 위해 EditAnywhere 로 선언함.
	// 
	// IMC같은 경우에는 원래는 하나라서 걍 플레이어에 밖았는데 이제 2개로 좀 나눠져가지고 아예 CharacterControlData 로 갔다.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuarterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	// 현재 캐릭터의 컨트롤 타입 확인용
	ECharacterControlType CurrentCharacterControlType;

	// 그리고 나서 각 입력 액션에 대해 매핑될 함수를 지정하는데, 여기서는 FInputActionValue 구조체를 받아야 하기 때문에
	// 이에 대한 헤더를 따로 선언해준다
	// + 이 역시 각각 Shoulder Quarter 로 변경
	//void Move(const FInputActionValue& Value);
	//void Look(const FInputActionValue& Value);

	void QuarterMove(const FInputActionValue& Value);
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void Attack();

// For HUD
protected:
	virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget) override;

};
