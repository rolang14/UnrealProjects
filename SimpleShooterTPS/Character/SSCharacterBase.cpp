// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SSCharacterBase.h"
#include "Components/CapsuleComponent.h"

#include "Gun/SSGunBase.h"

#include "Game/SSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASSCharacterBase::ASSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	FireTimer = 0.f;
	bIsDead = false;
}

void ASSCharacterBase::Tick(float DeltaTime)
{
	// 총기 연사력
	if (FireTimer < FireRate)
	{
		// 발사가 가능하지 않으면 계속 타이머 증가
		FireTimer += DeltaTime;
	}

	// HP Smoothing
	if (CurrentSmoothHp != CurrentHp)
	{
		CurrentSmoothHp = FMath::InterpEaseOut(CurrentSmoothHp, CurrentHp, DeltaTime / 20.f, 90.f);
	}
}

float ASSCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= DamageApplied;
	CurrentHp = FMath::Clamp(CurrentHp, 0.f, MaxHp);

	// 사망 상태로 변경
	if (CurrentHp == 0 && !GetDead())
	{
		ProcessPawnDead();
	}

	return DamageApplied;
}

// Called when the game starts or when spawned
void ASSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHp = MaxHp;

	if (GunClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Gun Actor : %s"), *GetName());
		return;
	}

	// 런타임에 액터 소환
	Gun = GetWorld()->SpawnActor<ASSGunBase>(GunClass);
	// 멀티플레이에서 액터의 Owner를 정의하는 것은 무엇보다도 중요하다.
	Gun->SetOwner(this);
	// 런타임에 액터를 컴포넌트에 부착하기.
	//		1. 일단 이 모델에서는 캐릭터에 총이 붙어 있으므로 그것을 찾아서 제거 (여기서는 본에 있음)
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	//		2. 원하는 컴포넌트의 소켓에 AttachToComponent (생성자에서는 SetupAttachment 이지만 여기선 다르다.)
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

	FireRate = 60 / Gun->GetFireRate();
}

void ASSCharacterBase::ProcessPawnDead()
{
	SetDead(true);

	if (DeathSound)	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	// 폰이 죽으면 해당 폰이 사망했음을 게임모드에게 알려 줌.
	ASSGameMode* const GameMode = GetWorld()->GetAuthGameMode<ASSGameMode>();
	if (GameMode)
	{
		GameMode->PawnKilled(this);
	}

	// Pawn에게서 안전하게 컨트롤러를 제거하는 함수
	// AI 역시 AI 컨트롤러를 제거함으로써 죽은 뒤 움직이지 않게 된다.
	DetachFromControllerPendingDestroy();
	// 남아있는 캡슐 컴포넌트의 충돌 설정을 제거
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASSCharacterBase::Fire()
{
	// GetWorld()->GetTimerManager() 을 사용하니까
	// 이것도 역시 Tick 을 베이스로 돌아가는지라, 프레임레이트에 독립적이지가 않았다.
	// 그래서 그냥 Tick 및 DeltaSeconds 를 이용해서 구현하는것이 정확함.
	if (Gun && FireTimer >= FireRate)
	{
		FireTimer = 0.f;
		Gun->PullTrigger();
	}
}

void ASSCharacterBase::Reload()
{
	if (Gun)
	{
		Gun->Reload();
	}
}
