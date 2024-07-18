// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TTBasePawn.generated.h"

UCLASS()
class TOONTANK_API ATTBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATTBasePawn();

protected:
	void RotateTurret(const FVector& TargetVector) const;
	void Fire();
	virtual void HandleDestruction();

private:
	// 상속되어지지 않고, 헤더 파일을 추가로 포함해야 하는 경우에, 헤더파일에는 되도록이면 전방선언 한다.
	// 그리고 되도록이면 스마트 포인터 사용하는게 메모리 관리 실수 방지에 좋다.
	// 또, 되도록이면 리플렉션 사용하는게 좋다.
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh Info")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh Info")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh Info")
	UStaticMeshComponent* TurretMesh;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ATTProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<class UParticleSystem> DeathParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<class USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
};
