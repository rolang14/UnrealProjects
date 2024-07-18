// Copyright NHE


#include "Character/AuraCharacterBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"

#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 다른 캐릭터에 의해 카메라가 막히는 일이 없도록 하자.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// FString 은 텍스트 조작을 위해 내부적으로 단순하지 않은 구조를 만든다. 때문에 TEXT()매크로가 필요하지만, FName은 그렇지 않으므로 단순히 ""로 정의해도 된다.
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffect);

	FGameplayEffectContextHandle GEContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GEContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GESpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, Level, GEContextHandle);
	// Self로 해도 되는데 한번 Target 도 써 보자.
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	// 어빌리티 획득 관련 기능은 서버에서만 작동해야 한다.
	if (!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AddCharacterAbilities(StartupAbilities);
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	// Weapon 이 없다면.. 문제가 생기므로 일단 check 를 해놓는다. 반드시 Weapon 이 설정되어 있어야 한다. 그것이 맨주먹일지라도
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

#pragma region For Animation Heading to Target

void AAuraCharacterBase::SetFacingTarget(const FVector& TargetLocation)
{
	check(MotionWarpingComponent);
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, TargetLocation);
}

#pragma endregion

#pragma region For Hit Reaction & Death

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	check(HitReactMontage);
	return HitReactMontage;
}

void AAuraCharacterBase::PerformDeath()
{
	// 사망 시 무기를 떨구기.
	// Detach 는 자동적으로 Replicated Action 을 수행한다.
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

// 여기서는 자동으로 Replicate 되지 않는 것들을 수행한다. 으레 RPC 함수들이 다 그런 목적이다.
void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	// Weapon 을 떨어뜨리기 위해 Physics 를 켜고 중력을 활성화한다.
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	// Collision 을 활성화하는데, 오버랩이나 이벤트 처리가 목적이 아니라 그냥 Physics 를 위한 Collision 이다.
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// Mesh의 경우 동일하게 하면 된다.
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// 그리고 나서 나중에는 Impulse 를 주어서 Ragdoll 이 피격된 방향으로 움직이도록 하고 싶다.

	// 추가로 Capsule Component 가 더이상 다른것들을 방해하지 않도록 Collision 을 꺼줘야 한다.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Perform Dissolve Effect
	DeathDissolve();
}

void AAuraCharacterBase::DeathDissolve()
{
	if (IsValid(DissolveMaterialOnDeath))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialOnDeath, this);
		// 만약 Material 이 여러개라면, 그리고 다 Dissolve 하고 싶다면 모든 머티리얼에 대해 Dissolve 머티리얼을 만들고, 이걸 슬롯별로 여러번 해야 한다.
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartCharacterDeathDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialOnDeath))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialOnDeath, this);
		// 만약 Material 이 여러개라면, 그리고 다 Dissolve 하고 싶다면 모든 머티리얼에 대해 Dissolve 머티리얼을 만들고, 이걸 슬롯별로 여러번 해야 한다.
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDeathDissolveTimeline(DynamicMatInst);
	}
}

#pragma endregion


#pragma region Virtual Functions (No meanings)

void AAuraCharacterBase::SetupAbilityInfo()
{
}

#pragma endregion


