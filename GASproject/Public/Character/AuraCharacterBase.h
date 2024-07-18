// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"


class UMotionWarpingComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;


// 추상 클래스로 만든다.
// IAbilitySystemInterface : Ability Getter 를 제공하는 심플한 인터페이스
// ICombatInterface : Level 을 반환하는 인터페이스
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	// 모든 캐릭터는 어빌리티를 갖고 있으며, 어빌리티의 셋업을 여기서 진행 (ASC, AS, Delegate Binding 등)
	virtual void SetupAbilityInfo();

#pragma region For Weapon

public:
	// 무기의 특정 소켓 위치 반환 (For Abilities)
	virtual FVector GetCombatSocketLocation() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// 보통 뭔가를 발사하거나 할때 다들 이렇게 TipSocket 을 쓴다고 함.
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

#pragma endregion

protected:
	// GAS를 사용하기 위해 ASC와 AS를 선언
	// AuraAbility로 선언하지 않고 상위 클래스로 선언하는 이유는
	// 
	// PlayerCharacter 에서는 PlayerState 를 통해 구현할 것이고
	// Enemy 에서만 캐릭터에 직접 구현할 것이기 때문이다.
	// 그러므로 Base 에서는 ASC와 AS를 직접 구현하지는 않는다.
	// 이렇게 포함은 하되, 각각 다른 방법으로 구현할 경우에는 굳이 Base에 구현할 필요가 없다.
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

protected:
	// GE 적용 함수
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect, const float Level) const;

	// 어트리뷰트 초기화.
	// Enemy 에서 다른 식으로 구현하기 위해 가상 함수로 변환
	virtual void InitializeDefaultAttributes() const;

	// Primary Attribute 초기화를 위한 GameplayEffect
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// Secondary Attribute 초기화를 위한 GameplayEffect
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// 모든 초기화 완료 후 Vital Attribute 초기화를 위한 GameplayEffect
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

#pragma region Gameplay Abilities

protected:
	virtual void AddCharacterAbilities();

private:
	// 처음부터 소유할 어빌리티
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Abilities", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

#pragma endregion

#pragma region For Animation Heading to Target

public:
	// Montage 의 Facing Target 설정
	virtual void SetFacingTarget(const FVector& TargetLocation) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Motion Warping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	// 회전하고 싶은 Montage 의 Warp Target 이름
	UPROPERTY(EditDefaultsOnly, Category = "Motion Warping")
	FName WarpTargetName;

#pragma endregion

#pragma region For Hit Reaction & Death

protected:
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	// 사망 시, 사망 모션이 없더라도 Ragdoll 상태로 만들면 그냥 죽은것처럼 된다.
	// 이렇게 Ragdoll 상태로 변경할 경우, 서버에서 수행 후 클라이언트로 가야 한다.
	// 그리고 사망 판정 역시, 클라이언트가 아니라 서버에서 수행해야 한다. 
	virtual void PerformDeath() override;
	// 이렇게 RPC를 정의하면, 해당 OwnerActor 와 관계 없이 반드시 서버에서 수행 후 클라이언트로 Replicate 된다.
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	// 사망 시 Material 을 Dissolve 로 변경하고 디졸브한다.
	void DeathDissolve();
	// 근데 디졸브는 문맥 상으로도, 생산성 측면에서도 블루프린트에서 하는게 훨씬 낫다. 그러므로 BlueprintImplementableEvent 로 정의한다.
	UFUNCTION(BlueprintImplementableEvent)
	void StartCharacterDeathDissolveTimeline(UMaterialInstanceDynamic* CharacterDynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDeathDissolveTimeline(UMaterialInstanceDynamic* WeaponDynamicMaterialInstance);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	// Dissolve Material when Die
	UPROPERTY(EditDefaultsOnly, Category = "Effect Material")
	TObjectPtr<UMaterialInstance> DissolveMaterialOnDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Effect Material")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialOnDeath;

#pragma endregion

};
