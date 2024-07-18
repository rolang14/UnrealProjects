// Copyright NHE


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// Projectile 을 소환하되, 서버에서 소환하고 클라이언트에 Replicated 되기를 원한다.
	// Projectile 자체도 Replicated 상태여야 하며, (bReplicates = true) GA에서는 HasAuthority 로 서버인지 확인할 수 있다.
	//if (!HasAuthority(&ActivationInfo)) return;
	// 여기엔 ActivationInfo 가 없으므로 Authority 를 다른 객체에서 찾는다.
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	// 어디에 Projectile 을 소환할 것인지?
	// 정확하게는 Staff 의 끝에 소환하고 싶은데, 그러려면 Character 를 알아야 한다.
	// 근데 이건 그냥 Spell 이라서 굳이 Character 를 알고 싶지 않다.
	// Character 를 모르더라도 그냥 그 캐릭터의 Weapon 의 특정 소켓 (스태프라면 스태프의 끝) 에 소환하고 싶다.
	// 바로 이럴 때 전략 패턴을 사용할 수 있겠다. 근데 우리는 이미 전투와 관련된 CombatInterface 를 갖고 있다. 그리고 이건 이미 CharacterBase 가 사용하고 있으므로 적절하다.
	FTransform SpawnTransform;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector FireLocation = CombatInterface->GetCombatSocketLocation();
		// FireLocation -> TargetLocation 방향
		FRotator FireRotation = (ProjectileTargetLocation - FireLocation).Rotation();
		// Pitch 를 0으로 두어서 바닥에 평평하게 날아가도록 한다.
		FireRotation.Pitch = 0.f;

		SpawnTransform.SetLocation(FireLocation);
		SpawnTransform.SetRotation(FireRotation.Quaternion());

		// 발사체가 날아가서 무언가에 맞으면, 발사체가 갖고 있는 Gameplay Effect 가 적용되었으면 좋겠다. 즉, 발사체에 GE(GE Spec)를 부여하고 싶다.
		// Spawn 하면서 이러한 것들을 설정할 수 있는 방법은 SpawnActorDeferred 를 통해서이다. 주석을 읽어 보면 give caller an opportunity to set parameters beforehand 라고 적혀 있다.
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// Apply GE to Projectile
		// Ability 에서 ASC를 얻어오는 방법
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

		// Projectile 에 넘겨 주기 전에, 추가로 아래와 같은 설정을 한다.
		// 1. 레벨에 맞는 AbilityDamage (by FScalableFloat with Curve Table) 를 가져온다.
		const float ScaledDamage = AbilityDamage.GetValueAtLevel(GetAbilityLevel());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Firebolt Damage : %f, Firebolt Level : %f"), ScaledDamage, GetAbilityLevel()));
		// 2. 대미지 부여를 위해 GameplayEffect 에서 사용 가능하도록 Damage Tag와 Damage 값을 설정 (For Set by Caller)
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FAuraGameplayTags::Get().Damage, ScaledDamage);

		// 이렇게 GE를 만들어서 Projectile 에 지정해 주면 끝
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
