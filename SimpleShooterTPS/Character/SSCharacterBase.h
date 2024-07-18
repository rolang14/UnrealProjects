// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SSCharacterAttackInterface.h"
#include "SSCharacterBase.generated.h"

UCLASS()
class SIMPLESHOOTER_API ASSCharacterBase : public ACharacter, public ISSCharacterAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASSCharacterBase();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetHpRatio() const { return CurrentSmoothHp / MaxHp; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDead(const bool InIsDead) { bIsDead = InIsDead; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimMontage* const GetReloadMontage() const { return ReloadMontage; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimMontage* const GetFireMontage() const { return FireMontage; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 폰 사망 시 프로세스 진행
	void ProcessPawnDead();

private:
	// 총에 관한 것은 모두 해당 클래스에서 관리함.
	UPROPERTY(EditDefaultsOnly, Category = "Gun Equipment")
	TSubclassOf<class ASSGunBase> GunClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASSGunBase> Gun;

	// 사망 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Voice")
	TObjectPtr<class USoundBase> DeathSound;

	// 캐릭터 별 재장전 모션
	UPROPERTY(EditDefaultsOnly, Category = "Reload Motion")
	UAnimMontage* ReloadMontage;

	// 캐릭터 별 총기 발사 모션
	UPROPERTY(EditDefaultsOnly, Category = "Fire Motion")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Character Stat")
	float MaxHp = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Character Stat")
	float CurrentHp = 0.f;
	
public:	
	virtual void Fire() override;
	virtual void Reload() override;

private:
	FTimerHandle FireTimerHandle;
	float FireRate;
	float FireTimer;

	// UI에 HP를 스무스하게 표현하기 위함
	float CurrentSmoothHp = 0.f;
	bool bIsDead;
};
