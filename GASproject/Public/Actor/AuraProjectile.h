// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;


// Ability Projectile 을 정의하기 위한 클래스
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// 이 Projectile 의 생존 시간
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Life Span")
	float LifeSpan = 10.f;

#pragma region For SFX/VFX

private:
	void PlayImpactEffects();

private:
	// For Impact VFX
	UPROPERTY(EditAnywhere, Category = "Impact Effect")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// For Impact SFX
	UPROPERTY(EditAnywhere, Category = "Impact Effect")
	TObjectPtr<USoundBase> ImpactSound;

	// For Looping SFX
	UPROPERTY(EditAnywhere, Category = "Impact Effect")
	TObjectPtr<USoundBase> LoopingSound;

private:
	// For Saving Spawned Looping SFX
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	bool bIsDestroyed = false;

#pragma endregion

public:
	// GE를 외부에서 적용하기 위해 (Ex: 어빌리티에서 이 Projectile을 소환할 때) public 으로 정의함
	// ExposeOnSpawn : 이 클래스 타입에 대한 액터 스폰(Spawn Actor) 노드에 프로퍼티를 노출해야 하는지 여부를 지정합니다.
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

};
