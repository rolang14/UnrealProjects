// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun/SSGunBase.h"
#include "Character/SSCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASSGunBase::ASSGunBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Transform"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASSGunBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = FullAmmo;
}

// Called every frame
void ASSGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReloading)
	{
		ReloadRatio += DeltaTime / ReloadTime;
	}
}

void ASSGunBase::PullTrigger()
{
	// 격발 불가 판정들
	if (bIsReloading) return;

	if (CurrentAmmo <= 0)
	{
		UGameplayStatics::SpawnSoundAttached(NoAmmoSound, GunMesh, TEXT("MuzzleFlashSocket"));
		return;
	}

	CurrentAmmo--;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return;
	}

	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr)
	{
		return;
	}

	PerformShotEffects(OwnerController);

	FVector ShotDirection;
	FHitResult BulletHitResult;
	if (IsShotHit(BulletHitResult, ShotDirection, OwnerController))
	{
		// Play Hit Sounds and Effects
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, BulletHitResult.ImpactPoint, ShotDirection.Rotation(), true);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, BulletHitResult.ImpactPoint);

		if (AActor* HitActor = BulletHitResult.GetActor())
		{
			FPointDamageEvent BulletDamageEvent(BulletDamage, BulletHitResult, ShotDirection, nullptr);
			// 대미지를 가한 최종 액터, 즉 캐릭터가 발사했다고 하더라도 결국 Projectile 에 의해 대미지가 발생한다면 그것은 Projectile 이 될 것이다.
			HitActor->TakeDamage(BulletDamage, BulletDamageEvent, OwnerController, this);
		}
	}
}

void ASSGunBase::Reload()
{
	if (!bIsReloading && CurrentAmmo < FullAmmo)
	{
		ReloadRatio = 0.f;
		bIsReloading = true;
		UE_LOG(LogTemp, Display, TEXT("Reloading ..."));

		// 재장전 SFX
		if (ReloadSound)
		{
			UGameplayStatics::SpawnSoundAttached(ReloadSound, GunMesh, TEXT("MuzzleFlashSocket"));
		}

		// 재장전 모션
		if (ASSCharacterBase* OwnerCharacter = Cast<ASSCharacterBase>(GetOwner()))
		{
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			UAnimMontage* const ReloadMontage = OwnerCharacter->GetReloadMontage();

			if (AnimInstance && ReloadMontage)
			{
				AnimInstance->Montage_Play(ReloadMontage);
			}
		}

		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ASSGunBase::ReloadGun, ReloadTime, false);
	}
}

void ASSGunBase::ReloadGun()
{
	CurrentAmmo = FullAmmo;
	bIsReloading = false;
	ReloadRatio = 0.f;
}

//void ASSGunBase::PlayFireSounds() const
//{
//	 이미 내부적으로 검사를 하기 때문에 아래 부분은 필요가 없다.
//	if (FireSound == nullptr)
//	{
//		UE_LOG(LogTemp, Error, TEXT("No FireSound Gun : %s"), *GetName());
//		return;
//	}
//}

bool ASSGunBase::IsShotHit(FHitResult& OutBulletHitResult, FVector& OutShotDirection, const AController* const InOwnerController) const
{
	// 총을 발사할 때 플레이어의 카메라가 보는 지점으로 Raytrace 하기 위해 카메라를 그려 본다.
	FVector PlayerViewLocation; FRotator PlayerViewRotation;
	// APlayerController->GetPlayerViewPoint 으로 플레이어의 현재 View (카메라) Location 및 Rotation 을 받아 온다.
	InOwnerController->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	// 총알이 어디서부터 날아왔는지를 알기 위해선, 위에서 사용했던 방향 벡터를 역으로 뒤집으면 된다.
	// 다만, (-1 * PlayerViewRotation) 이거랑 (-PlayerViewRotation.Vector()).Rotation() 이거랑은 다르다.
	// Rotator 에 직접 -1 을 곱하는 것은 방향을 반대로 뒤집는게 아니라, 각 Pitch Yaw Roll 을 역으로 뒤집는것과 같다. 그러니까 좌우 반전인 셈이다.
	OutShotDirection = -PlayerViewRotation.Vector();

	// 총알 Line Trace
	FVector BulletRange = PlayerViewLocation + PlayerViewRotation.Vector() * FireRange;

	// 총과 Owner 를 Line Trace 대상에서 제외
	FCollisionQueryParams ShootParams(TEXT("Shoot"), false, GetOwner());
	ShootParams.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(OutBulletHitResult, PlayerViewLocation, BulletRange, ECollisionChannel::ECC_GameTraceChannel1, ShootParams);
}

void ASSGunBase::PerformShotEffects(AController* const InPlayerController) const
{
	// Play Sounds and Effects
	// SpawnSound 는 대개 플레이어와 같이 소리가 움직이는 경우 (나중에 이 Spawn 한 컴포넌트를 Auto Destroy 하지 않고 사용할 수도 있기 때문에, 이와 같은 경우에는 오버헤드가 비교적 덜 발생한다) 사용하며
	// 반대로 아래에서와 같이 PlaySound 는 반대로 해당 컴포넌트가 다른 액터에 붙는 경우, 해당 액터를 막 찾아서 억지로 갖다붙이는것보다 그냥 Play 하고 없애는게 오버헤드가 적으므로 이렇게 사용한다.
	UGameplayStatics::SpawnSoundAttached(FireSound, GunMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));

	APlayerController* PlayerController = Cast<APlayerController>(InPlayerController);
	if (PlayerController == nullptr)
	{
		return;
	}

	if (ShotCameraShake)
	{
		PlayerController->ClientStartCameraShake(ShotCameraShake);
	}

	// 격발 모션
	if (ASSCharacterBase* OwnerCharacter = Cast<ASSCharacterBase>(GetOwner()))
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		UAnimMontage* const FireMontage = OwnerCharacter->GetFireMontage();

		if (AnimInstance && FireMontage)
		{
			AnimInstance->Montage_Play(FireMontage);
		}
	}
	
	// 총기 반동
	PlayerController->AddPitchInput(-FMath::RandRange(FireRecoilPitch, FireRecoilPitch * 1.5f));
	PlayerController->AddYawInput(-FMath::RandRange(FireRecoilYaw, FireRecoilYaw * 1.5f));
}