// Fill out your copyright notice in the Description page of Project Settings.

#include "Prop/TTProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATTProjectile::ATTProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;

	TrailParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Trace Particle"));
	TrailParticleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATTProjectile::BeginPlay()
{
	Super::BeginPlay();

	// OnComponentHit 델리게이트에 함수 바인딩
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ATTProjectile::OnHit);
	if (LaunchSound) UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void ATTProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &HitResult)
{
	AActor *OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
		return;

	// Hit된 컴포넌트가 존재하고, 그게 자기 자신(발사체)가 아니면서 발사한 액터(발사체의 Owner) 도 아닐 때 실행
	if (OtherActor && OtherActor != this && OtherActor != OwnerActor)
	{
		// 적을 맞춘 발사체 HitComp 가 맞춘 OtherActor 가 대미지를 입어야 한다. Damage 만큼, 그리고 공격한 컨트롤러 (현재 발사체의 Owner 의 컨트롤러)
		// 및 DamageCauser (현재 발사체가 대미지를 주었으므로), 마지막 DamageType 은 아직 우리가 뭔가 정의하지 않았으므로 TSubclassOf을 쓰기보단 StaticClass 으로 정적으로 넘겨준다.
		UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerActor->GetInstigatorController(), this, UDamageType::StaticClass());
		if (HitSound) UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		if (HitParticle) UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation());
		if (HitCameraShakeClass) UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(HitCameraShakeClass);
		// or GetWorld()->GetFirstPlayerController()
	}
	// 그리고 탄환을 삭제한다.
	Destroy();
}