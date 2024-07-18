// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RBPlayerController.generated.h"

// 전방선언
class UInputMappingContext;
class UInputAction;
class URBInteractData;

struct FInputActionValue;


#pragma region Wheel Mode

// 마우스 휠 작동 모드 변경용 델리게이트 및 래퍼
DECLARE_DELEGATE_OneParam(FOnWheelModeChangedDelegate, const FInputActionValue&/*Input Value*/)
USTRUCT(BlueprintType)
struct FWheelModeChangedDelegateWrapper
{
	GENERATED_BODY()

	FWheelModeChangedDelegateWrapper() {};
	FWheelModeChangedDelegateWrapper(const FOnWheelModeChangedDelegate& InWheelModeChangedDelegate) : WheelModeChangedDelegate(InWheelModeChangedDelegate) {}

	FOnWheelModeChangedDelegate WheelModeChangedDelegate;
};

// 휠 작동 모드
// ZOOM 인 경우 : 줌 인/아웃
// INTERACT 인 경우 : 선택할 상호작용 변경
UENUM(BlueprintType)
enum class EMouseWheelMode : uint8
{
	ZOOM = 0,
	INTERACT = 1
};

#pragma endregion


#pragma region Player Mode

// 플레이어 모드 변경용 델리게이트 및 래퍼
DECLARE_DELEGATE(FOnPlayerModeChangedDelegate)
DECLARE_DELEGATE_OneParam(FOnPlayerModeChangedEvent, const EPlayerMode&/*Changed Mode*/)
USTRUCT(BlueprintType)
struct FPlayerModeChangedDelegateWrapper
{
	GENERATED_BODY()

	FPlayerModeChangedDelegateWrapper() {};
	FPlayerModeChangedDelegateWrapper(const FOnPlayerModeChangedDelegate& InPlayerModeChangedDelegate) : PlayerModeChangedDelegate(InPlayerModeChangedDelegate) {}

	FOnPlayerModeChangedDelegate PlayerModeChangedDelegate;
};

// 현재 플레이어의 상태
// FIELD_NORMAL = 필드에 있는 상태
// FIELD_INTERACTING = 필드에 있으면서 상호작용 중인 상태 (다이얼로그 재생 등)
// BATTLE_NORMAL = 전투에 진입한 상태
// BATTLE_BOOST = 전투 중에 BOOST 모드에 진입한 상태
UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	FIELD_NORMAL = 0,
	FIELD_INTERACTING,
	BATTLE_NORMAL,
	BATTLE_BOOST
};

#pragma endregion


/**
 * 캐릭터를 조작하는 컨트롤러의 기본 클래스
 * 조작해야 할 캐릭터가 총 4개까지이기 때문에
 * Input Binding 을 컨트롤러에서 진행한다.
 */
UCLASS()
class PROJECTRB_API ARBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARBPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;

	virtual void SetupInputComponent() override;

#pragma region Input Mapping Section

private:
	UPROPERTY(EditDefaultsOnly, Category = "InputMappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> WheelAction;

	// 걷기/달리기 변경 키 (향후 이동/스프린트 로 변경 가능성 있음)
	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> ChangeRunModeAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> FieldAttackAction;

	// 캐릭터 변경 키
	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> ChangeCharacterAction;

	// UI Section
	// 상호작용 선택 키
	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> InteractAction;

#pragma endregion

#pragma region Input Function Section

private:
	void SetCharacterEnhancedInput();

	// For Character Action Bind
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Wheel(const FInputActionValue& Value);
	void Interact();
	void Jump();
	void StopJumping();
	void ChangeRunMode();
	void FieldAttack();
	void SwitchActiveCharacter();
	void RunInteract();
	void ProcessDialogue();

	// Wheel Actions
	void Zoom(const FInputActionValue& Value);
	void ChangeInteract(const FInputActionValue& Value);

#pragma endregion

#pragma region Player Mode Change Section

public:
	FOnPlayerModeChangedEvent PlayerModeChangedDelegate;
	// InteractData Changed Delegate Bind Function
	void ChangeWheelMode(const TArray<const URBInteractData*>& InteractDataList);

	// 플레이어 모드 설정
	FORCEINLINE void SetCurrentPlayerMode(EPlayerMode InPlayerMode) { CurrentPlayerMode = InPlayerMode; PlayerModeChangedDelegate.ExecuteIfBound(CurrentPlayerMode); }
	FORCEINLINE const EPlayerMode GetCurrentPlayerMode() const { return CurrentPlayerMode; }

private:
	// 휠 작동 모드 (줌 인 or 상호작용 선택)
	UPROPERTY()
	TMap<EMouseWheelMode, FWheelModeChangedDelegateWrapper> WheelModeChangedActions;
	EMouseWheelMode WheelMode;

	// 현재 플레이어의 상태
	UPROPERTY()
	TMap<EPlayerMode, FPlayerModeChangedDelegateWrapper> PlayerModeChangedActions;
	EPlayerMode CurrentPlayerMode;

#pragma endregion

#pragma region HUD Section

public:
	FORCEINLINE TObjectPtr<UUserWidget> GetOnFieldHUD() const { return OnFieldHUD; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "On Field HUD")
	TSubclassOf<UUserWidget> OnFieldHUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "On Field HUD", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> OnFieldHUD;
};

#pragma endregion