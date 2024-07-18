// Copyright NHE


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRootComponent"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass, const EEffectRemovalPolicy& InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove)
{
	// 만약 아이템을 사용하는 모든 액터에 반드시 AS인터페이스를 구현할 것이 아니라면 아래의 코드를 사용해도 된다.
	// 들어가 보면 알겠지만, 아래 함수는 인터페이스 확인이 안되면 곧바로 FindComponent를 하기 때문에 ASC를 찾을 수 있다.
	//UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	
	// 하지만 여기서는 Interface를 무조건 쓴다고 생각하고 아래의 코드를 사용한다.
	IAbilitySystemInterface* TargetAbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
	if (TargetAbilitySystemInterface)
	{
		// 우선 Gameplay Effect 가 있는지를 반드시 확인해야 한다.
		// 이게 다 이거 적용하려고 하는건데 이게 없으면 의도가 아닌 실수이므로 Assert 한다.
		check(InGameplayEffectClass);

		// 그 다음은 ASC를 가져와서 GameplayEffect 를 적용하면 된다.
		UAbilitySystemComponent* TargetASC = TargetAbilitySystemInterface->GetAbilitySystemComponent();
		// GameplayEffect 를 적용하려면 Ability System Component 에서 할 수 있는데,
		// 적용하기 전에 적용할 Effect Spec를 만들어야 한다. (Spec이 아니라 Effect 자체를 적용하려 해도 마찬가지로 GameplayEffect를 만들어야 한다.)
		// 이를 만드려면 다시 만들놈의 서브클래스가 필요한데 우리는 바로 이것을 디자이너가 만들기를 유도하고, 이걸 넣음으로써 그것에 맞는 객체를 생성하게 되는 것이다.
		// 또 Level이 필요한데, 모든 GameplayEffect에는 Level이 존재한다. 나중에 필요할 때 다시 레벨을 적용할 수 있으므로 지금은 그냥 1이라고만 해놓는다.
		// 그리고 Context Handle이 필요한데.. Context 라고 하면.. 뭘까? GE에서는 뭐 누가 공격했고, 누가 맞았고, 무슨 스킬이고 이런것들이겠지? 겁나 많은데...
		// 어쩄든 이런 Context를 관리하는 핸들을 넘겨야 한다는 것이다. 자세한건 정의를 타고 들어가서 보자. 들어가서 보면 FGameplayEffectContext를 관리하는 래퍼 클래스일 뿐이다. Context는 Instigator의 여러 정보를 담아놓고 있는 것으로 보인다.
		FGameplayEffectContextHandle GEContextHandle = TargetASC->MakeEffectContext();
		// 그리고 어떤 오브젝트가 이 이펙트를 일으켰는지를 넣는 것이 좋다. 만약 스킬이나 어빌리티에 의한 효과라면, Instigator를 Add 하면 된다.
		GEContextHandle.AddSourceObject(this);

		// 마지막으로 Spec을 인자로 넣기 위한 SpecHandle를 만든다. 이것 역시 GameplayEffect를 관리하는 래퍼 클래스이다.
		const FGameplayEffectSpecHandle GESpecHandle = TargetASC->MakeOutgoingSpec(InGameplayEffectClass, ActorLevel, GEContextHandle);

		// 물론 이펙트를 적용하는 방법도 굉장히 여러 가지가 존재하지만, 일단은 단순한것부터 해 보자.
		// SpecHandle은 래퍼 클래스이고, 내부에 Data를 TSharedPtr로 갖고 있지만 실제로는 RawPointer 이다. 그리고 이 함수에서는 레퍼런스를 원하기 때문에 다시 원시 포인터에서 디레퍼런싱해서 끄집어 내야 한다.
		const FActiveGameplayEffectHandle ActiveGEHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());

		// Infinite GE인 경우, 명시적으로 삭제해 주어야 하는데 (여기서는 EndOverlap 에서 삭제한다.)
		// 그러기 위해서는 미리 GE에 대한 핸들을 저장해 놓아야 한다.
		// 핸들은 단순히 래퍼 클래스이므로 그 데이터에 접근하여 (실질적인 GE Spec)
		// 다시 GE Spec 은 SharedPtr 이므로 Raw 포인터로 변환하여 Def을 가져와야 진짜 Gameplay Effect에 접근한 것이다.
		const bool bIsInfinite = GESpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovalPolicy != EEffectRemovalPolicy::DoNotRemove)
		{
			// Infinite GE이면 액티브 핸들을 저장한다.
			// 굳이 AActor 와 짝지을 필요 없이, 이미 AbilitySystemComponent 를 알고 있으므로 이것과 짝지으면 다음부턴 Actor에서 파고들어갈 필요가 없다.
			ActiveInfiniteGEHandles.Add(ActiveGEHandle, TargetASC);
		}
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for (auto& InstantGE : InstantGameplayEffects)
	{
		if (InstantGE.InstantEffectApplyPolicy == EEffectApplyPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, InstantGE.InstantGameplayEffectClass);
		}
	}

	for (auto& DurationGE : DurationGameplayEffects)
	{
		if (DurationGE.DurationEffectApplyPolicy == EEffectApplyPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, DurationGE.DurationGameplayEffectClass);
		}
	}

	for (auto& InfiniteGE : InfiniteGameplayEffects)
	{
		if (InfiniteGE.InfiniteEffectApplyPolicy == EEffectApplyPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, InfiniteGE.InfiniteGameplayEffectClass, InfiniteGE.InfiniteEffectRemovalPolicy);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (auto& InstantGE : InstantGameplayEffects)
	{
		if (InstantGE.InstantEffectApplyPolicy == EEffectApplyPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, InstantGE.InstantGameplayEffectClass);
		}
	}

	for (auto& DurationGE : DurationGameplayEffects)
	{
		if (DurationGE.DurationEffectApplyPolicy == EEffectApplyPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, DurationGE.DurationGameplayEffectClass);
		}
	}

	for (auto& InfiniteGE : InfiniteGameplayEffects)
	{
		if (InfiniteGE.InfiniteEffectApplyPolicy == EEffectApplyPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, InfiniteGE.InfiniteGameplayEffectClass, InfiniteGE.InfiniteEffectRemovalPolicy);
		}
	}

	// 삭제 정책에서
	for (auto& InfiniteGE : InfiniteGameplayEffects)
	{
		if (InfiniteGE.InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			IAbilitySystemInterface* TargetAbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetActor);
			if (TargetAbilitySystemInterface == nullptr) return;
			{
				UAbilitySystemComponent* TargetASC = TargetAbilitySystemInterface->GetAbilitySystemComponent();
				if (!IsValid(TargetASC)) return;

				// Map에 저장된 핸들의 소유자를 확인하고 삭제한다.
				// 다만, Map 순회 중에 내부 멤버를 삭제할 수 없으므로 지워야 하는 놈만 따로 저장한다.
				TArray<FActiveGameplayEffectHandle> ActiveInfiniteGEHandleToRemove;
				for (auto& HandlePair : ActiveInfiniteGEHandles)
				{
					if (HandlePair.Value == TargetASC)
					{
						// Stack 은 -1 인 경우 모든 스택을 지워버린다.
						TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, InfiniteGE.RemovalStack);
						ActiveInfiniteGEHandleToRemove.Add(HandlePair.Key);
					}
				}
				// Map에서의 삭제
				for (FActiveGameplayEffectHandle& RemoveHandle : ActiveInfiniteGEHandleToRemove)
				{
					ActiveInfiniteGEHandles.FindAndRemoveChecked(RemoveHandle);
				}
			}
		}
	}
}

