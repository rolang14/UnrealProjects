// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RBCharacterBase.h"

#include "Interface/RBCharacterMovementInterface.h"
#include "Interface/RBCharacterWidgetInterface.h"

#include "RBCharacterPlayer.generated.h"

// 전방선언
class ARBPlayerController;

class USpringArmComponent;
class UCameraComponent;

class URBCharacterControlData;
class URBCharacterInteractComponent;
class URBInteractWidget;

enum class EPlayerMode : uint8;

struct FInputActionValue;

/**
 * 플레이어가 조작할 수 있는 캐릭터의 기본 클래스
 */
UCLASS()
class PROJECTRB_API ARBCharacterPlayer : public ARBCharacterBase, public IRBCharacterMovementInterface, public IRBCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	ARBCharacterPlayer();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

// For Character Action Section
private:
	void SetCharacterControlData();

	// Character Movement Interface
	virtual void InputActionLook(const FInputActionValue& Value) override;
	virtual void InputActionMove(const FInputActionValue& Value) override;
	virtual void InputActionZoom(const FInputActionValue& Value) override;
	virtual void InputActionJump() override;
	virtual void InputActionStopJumping() override;
	virtual void InputActionChangeRunMode() override;
	virtual void InputActionFieldAttack() override;

	// Change Movement Mode for Delegate
	UFUNCTION()
	void ChangeCharacterMovementMode(const EPlayerMode& PlayerMode);
	// Character Move Functions
	void ProcessCharacterMove(const float& MoveForwardValue, const float& MoveRightValue);

#pragma region Mouse Wheel Action Section

	// Camera Zoom Functions (Wheel 1st Action)
	void SetNextZoom(const float ZoomValue);
	void PerformSmoothZoom();

	// For Smooth Zoom Tick
	float NextZoom;

	// Selected Interact Change Functions (Wheel 2nd Action)
	virtual void InputActionChangeInteract(const FInputActionValue& Value) override;

#pragma endregion

	// Field Attack Functions
	void ProcessFieldAttack();

// For Character Based Widget Setup Section
private:
	virtual void SetupInteractWidget(URBInteractWidget* InUserWidget) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Character Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Character Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	// 캐릭터의 상호작용 관리용 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Character Interact")
	TObjectPtr<URBCharacterInteractComponent> CharacterInteractComponent;

	// 캐릭터의 각종 컨트롤 세팅 데이터
	// 필드용
	UPROPERTY(EditDefaultsOnly, Category = "Character Control Data")
	TObjectPtr<URBCharacterControlData> OnFieldCharacterControlData;
	// 전투용
	UPROPERTY(EditDefaultsOnly, Category = "Character Control Data")
	TObjectPtr<URBCharacterControlData> InBattleCharacterControlData;
	// 현재 데이터
	UPROPERTY()
	TObjectPtr<URBCharacterControlData> CurrentCharacterControlData;

	// 델리게이트 해제를 위한 이전 컨트롤러 저장용
	UPROPERTY()
	TObjectPtr<ARBPlayerController> ExPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Character Animation Blueprint", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> OnFieldAnimationBP;

	UPROPERTY(EditDefaultsOnly, Category = "Character Animation Blueprint", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> InBattleAnimationBP;

	// Settings for Camera Movement
	UPROPERTY(EditAnywhere, Category = "Camera")
	float RotateSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmMaxLength = 600.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmMinLength = 100.f;

	// 줌할때 얼굴이 잘 안보여서 하는 설정
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraHeight = 30.f;

	bool IsRunMode = true;
};
