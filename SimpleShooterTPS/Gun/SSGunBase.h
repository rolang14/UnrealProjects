// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSGunBase.generated.h"

UCLASS()
class SIMPLESHOOTER_API ASSGunBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASSGunBase();

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetFullAmmo() const { return FullAmmo; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetReloadRatio() const { return ReloadRatio; }

	FORCEINLINE const float GetFireRate() const { return FireRate; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();
	void Reload();

private:
	bool IsShotHit(FHitResult& OutBulletHitResult, FVector& OutShotDirection, const AController* const InOwnerController) const;
	void PerformShotEffects(AController* const InPlayerController) const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Gun Mesh")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Gun Mesh")
	TObjectPtr<class USkeletalMeshComponent> GunMesh;

	// 총기 발사 효과음
	UPROPERTY(EditAnywhere, Category = "Gun Effect")
	TObjectPtr<class USoundBase> FireSound;

	UPROPERTY(EditAnywhere, Category = "Gun Effect")
	TObjectPtr<class USoundBase> NoAmmoSound;

	// 총기 발사 시 총열 격발 이펙트
	UPROPERTY(EditAnywhere, Category = "Gun Effect")
	TObjectPtr<class UParticleSystem> MuzzleFlash;

	// 해당 총기에 피격 시의 사운드
	UPROPERTY(EditAnywhere, Category = "Gun Effect")
	TObjectPtr<class USoundBase> HitSound;

	// 해당 총기에 피격 시의 이펙트
	UPROPERTY(EditAnywhere, Category = "Gun Effect")
	TObjectPtr<class UParticleSystem> HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> ShotCameraShake;

	// 재장전 진행 중의 사운드
	UPROPERTY(EditAnywhere, Category = "Gun Effect")
	TObjectPtr<class USoundBase> ReloadSound;

	// Fire Rate stands for Rifle's RPM. Rounds Per Minute
	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	float FireRate = 300.f;

	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	float FireRange = 1000.f;

	// 총기 반동 Pitch Yaw
	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	float FireRecoilPitch = 1.f;

	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	float FireRecoilYaw = 1.f;

	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	float BulletDamage = 20.f;

	// 총기 탄창과 잔탄
	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	int32 FullAmmo = 30;

	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	int32 CurrentAmmo = 0;

	// 총기 재장전 시간
	UPROPERTY(EditAnywhere, Category = "Gun Stat")
	float ReloadTime = 5.f;

private:
	void ReloadGun();

	FTimerHandle ReloadTimerHandle;
	float ReloadRatio;
	bool bIsReloading = false;
};
