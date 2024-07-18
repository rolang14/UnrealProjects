// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTProjectile.generated.h"

class USoundBase;

UCLASS()
class TOONTANK_API ATTProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATTProjectile();

	// virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent *ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile Effects")
	TObjectPtr<class UParticleSystem> HitParticle;

	UPROPERTY(VisibleAnywhere, Category = "Projectile Effects")
	TObjectPtr<class UParticleSystemComponent> TrailParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile Effects")
	TObjectPtr<USoundBase> LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Projectile Effects")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "Projectile Effects")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage = 50.f;
};