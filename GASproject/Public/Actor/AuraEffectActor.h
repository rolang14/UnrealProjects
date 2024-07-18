// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;

#pragma region For Gameplay Effect

// Effect 적용 규칙. 이것을 보고 이펙트를 적용한다.
//
// @ ApplyOnOverlap : 오버랩 시작 시 적용
// @ AllpyOnEndOverlap : 오버랩 종료 시 적용
// @ DoNotApply : 적용하지 않음
UENUM(BlueprintType)
enum class EEffectApplyPolicy : uint8
{
	ApplyOnOverlap = 0,
	ApplyOnEndOverlap,
	DoNotApply
};

// Effect 제거 규칙. 이것을 보고 이펙트를 제거한다. (거의 Infinite Effect 에만 사용한다.)
// 
// @ RemovalOnEndOverlap : 오버랩 종료 시 삭제
// @ DoNotRemove : 삭제하지 않음
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap = 0,
	DoNotRemove
};


USTRUCT(BlueprintType)
struct FInstantGameplayEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere)
	EEffectApplyPolicy InstantEffectApplyPolicy = EEffectApplyPolicy::DoNotApply;
};

USTRUCT(BlueprintType)
struct FDurationGameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere)
	EEffectApplyPolicy DurationEffectApplyPolicy = EEffectApplyPolicy::DoNotApply;
};

USTRUCT(BlueprintType)
struct FInfiniteGameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere)
	EEffectApplyPolicy InfiniteEffectApplyPolicy = EEffectApplyPolicy::DoNotApply;

	UPROPERTY(EditAnywhere)
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY(EditAnywhere)
	float RemovalStack = 1;
};

#pragma endregion


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

protected:
	// Effect 적용 함수
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass, const EEffectRemovalPolicy& InfiniteEffectRemovalPolicy);

	// Apply, Removal Policy 에 따라 GE를 적용할지 적용하지 않을지, 또 삭제할지 삭제하지 않을지 결정하기 위한 함수.
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

protected:
	// 적용할 Gameplay Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Setting")
	TArray<FInstantGameplayEffect> InstantGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Setting")
	TArray<FDurationGameplayEffect> DurationGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Setting")
	TArray<FInfiniteGameplayEffect> InfiniteGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Setting")
	float ActorLevel = 1.f;

private:
	// Active GE (Infinite) Handle 저장용 맵
	// 자료구조로 저장할 때, 레퍼런스는 전방선언 가능하지만 (포인터 사이즈는 동일하므로)
	// 구조체는 그 크기를 알 수 없기 때문에 전방선언 할 수 없으며, 헤더를 포함해야만 한다.
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveInfiniteGEHandles;
};
