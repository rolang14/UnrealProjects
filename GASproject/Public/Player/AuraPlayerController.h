// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class IHighlightInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;

struct FInputActionValue;
struct FGameplayTag;

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void SetupInputSystem();
	void CursorChase();

private:
	// 전략 패턴을 사용할 때 Interface 를 멤버로 갖기 위해서는 Interface* 를 포함할 수 있는 래퍼 클래스를 사용해야 한다.
	// TScriptInterface 를 사용하면 된다.
	TScriptInterface<IHighlightInterface> LastHighlightedActor;
	TScriptInterface<IHighlightInterface> CurHighlightActor;
	FHitResult CursorHit;
	
#pragma region Input Action

private:
	void Move(const struct FInputActionValue& InputActionValue);
	FORCEINLINE void ShiftPressed() { bIsShiftPressed = true; }
	FORCEINLINE void ShiftReleased() { bIsShiftPressed = false; }

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	bool bIsShiftPressed = false;

#pragma endregion

#pragma region For Binding Abilities & Callbacks

private:
	// 만약 Tick이나 Hold Functoin 처럼 Cast를 프레임마다 계속 해야해서 코스트 문제가 있다면, 아래처럼 Getter 를 만들어서 사용하면 된다.
	UAuraAbilitySystemComponent* GetAuraASC();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHold(FGameplayTag InputTag);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;

#pragma endregion

#pragma region For Click to Move Pathfinding

private:
	void PerformAutoRun();

private:
	// 목적지 위치 (마우스 클릭 Pos) 캐싱용
	FVector CachedDestination = FVector::ZeroVector;
	// 마우스 버튼 홀드 타임
	float FollowTime = 0.f;
	// 위의 Threshold
	float ShortPressThreshold = 0.5f;
	// AutoRunning 중인지
	bool bIsAutoRunning = false;
	// 마우스를 클릭했을 때, 타게팅이 되어 있는지 아니면 타게팅이 안되어있어서 AutoRun 을 하면 되는건지
	bool bIsTargeting = false;

	// 목적지의 이 오차 내로 도달하면 도착했다고 본다.
	UPROPERTY(EditDefaultsOnly)
	float AutoRunCompleteRadius = 50.f;

	// Spline for Pathfinding
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> AutoRunningSpline;

#pragma endregion

#pragma region Show Damage UI

public:
	// 대미지 표시 함수. DamageAmount : 표시할 대미지 양, Target : 표시할 캐릭터 타겟
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float DamageAmount, ACharacter* Target);

protected:
	// 대미지 표시를 위한 UI
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UDamageTextComponent> DamageTextComponent;

#pragma endregion




};
