// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/TTBasePawn.h"
#include "Prop/TTProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATTBasePawn::ATTBasePawn()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ATTBasePawn::RotateTurret(const FVector &TargetVector) const
{
	FVector ToTarget = TargetVector - TurretMesh->GetComponentLocation();
	FRotator TargetRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			TargetRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			1.f)
		);
}

void ATTBasePawn::Fire()
{
	// DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 20.f, 12, FColor::Red, false, 3.f);
	ATTProjectile* Projectile = GetWorld()->SpawnActor<ATTProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	// On Hit 에서 Owner 가 필요하기 때문에 SetOwner 해줌. Const 함수에서는 this를 캐스팅할 수가 없기 때문에 에러가 나더라.
	Projectile->SetOwner(this);
}

void ATTBasePawn::HandleDestruction()
{
	// Add Destroy Sounds And Effects.
    if (DeathParticle) UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	if (DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	if (DeathCameraShakeClass)
	{
		// Effects Only Player Died.
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController) PlayerController->ClientStartCameraShake(DeathCameraShakeClass);

		// Effects Any BasePawn Died.
		// GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}