// Copyright NHE


#include "Actor/AuraProjectile.h"
#include "Aura/Aura.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


// Sets default values
AAuraProjectile::AAuraProjectile()
{
	// Projectile 을 소환하되, 서버에서 소환하고 클라이언트에 Replicated 되기를 원한다.
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	SphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereCollider);

	// Not for Simulate Something
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Set Collision Type to Projectile
	SphereCollider->SetCollisionObjectType(ECC_Projectile);
	SphereCollider->SetGenerateOverlapEvents(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnBeginOverlap);

	// 어빌리티에 지속되는 사운드를 추가하고, 이를 LoopingSoundComponent 로 저장해놓을 수 있다.
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Projectile Hit"));

	// Play SFX, VFX
	PlayImpactEffects();

	// 그리고 서버에서 삭제한다.
	// 만약 클라이언트에서 먼저 SFX/VFX 를 재생했다면 문제 없지만
	// 반대로 서버가 먼저 삭제해버리면 클라이언트에서 SFX/VFX 재생이 안된다.
	if (HasAuthority())
	{
		// 서버일 경우에만 GE를 적용한다.
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}

		Destroy();
	}
	else
	{
		// 여기까지 도달하고 bIsDestroyed 가 true 되었다면, SFX/VFX를 재생한 것.
		bIsDestroyed = true;
	}
}

void AAuraProjectile::Destroyed()
{
	// 재생 전에 서버에 의해 파괴되었다면
	if (!bIsDestroyed && !HasAuthority())
	{
		// 재생
		PlayImpactEffects();
	}

	Super::Destroyed();
}

void AAuraProjectile::PlayImpactEffects()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	// Impact 되면 지속되는(ex : 날아가는) 소리를 멈춘다.
	LoopingSoundComponent->Stop();
}