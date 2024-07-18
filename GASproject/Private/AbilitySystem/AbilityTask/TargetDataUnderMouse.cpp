// Copyright NHE


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

// 이 Task 는 어빌리티 발동 시 마우스 커서 아래의 타겟 위치 정보를 가져오기 위함이다..
void UTargetDataUnderMouse::Activate()
{
	// 현재 어빌리티를 사용하는 Actor 가 Locally Controlled 되고 있는지
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		// We are On the Server. So listen for Target Data.
		// Activate() 와 TargetDataDelegate 둘 중 뭐가 먼저 서버에 도달할지 알 수 없으나, Activate() 가 먼저 도달하는 경우 아래와 같이 델리게이트 바인딩함으로써, 이후에 데이터를 받을 수 있다.
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		// 반대로, TargetDataDelegate 가 만약 먼저 발생했다면, 아직 Bind 하기 전이기 때문에 데이터를 받을 수 없는데, 이런 경우 아래와 같이 뒤늦게 Activate 를 호출하고 델리게이트를 바인딩하더라도
		// 다시 재콜백하여 데이터를 줄 수 있다. 이 때 TargetData 가 벌써 Set 되어있는지를 확인하고 다시 델리게이트를 보낸다.
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, ActivationPredictionKey);
		// 만약 재콜백이 이루어지지 않았다면
		if (!bCalledDelegate)
		{
			// ㄱㄷ
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// Scoped Prediction 을 현재 Scope 로 세팅한다.
	// Scope 는 말 그대로 현재 이 블록을 의미하고, 이 안에서 일어나는 일을 Predict 하기 위해 설정한다.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// 로컬에서 Cursor Hit 을 계산하고
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);

	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	// 서버에 데이터를 넘기기 위해서 FGameplayAbilityTargetData_SingleTargetHit 빌트인 타입에다가 데이터를 저장한다.
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	// 서버로 데이터를 넘기기 위해서는 ASC가 필요하다. 아래의 함수를 호출함으로써 Server 에 Replicated 될 TargetData 로 우리의 TargetData 가 설정이 된다.
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);
	// 근데 또 Broadcast 하기 전에 아래의 함수를 필수적으로 호출해야 한다.
	// 왜 이런걸 해야 하냐면, Ability Task Delegate 를 Broadcast 하면 안되는 상황이 있기 때문이라고 한다. 수많은 상황에 의해 Ability 가 Active 한 상태가 아니게 될 수 있기 때문이다. Async Task의 숙명이다.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnReceivedTargetData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& InHandle, FGameplayTag InActivationTag)
{
	// 이제 데이터 처리를 했으므로(DataHandle) Replicate 를 위해 캐싱된 TargetData 를 없앤다.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	// 이 다음으로는 클라이언트에서와 동일하게 Execute Delegate 를 실행시키기 위해 데이터를 가지고 BroadCast 한다.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnReceivedTargetData.Broadcast(InHandle);
	}
}
