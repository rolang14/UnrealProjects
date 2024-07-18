// Copyright NHE


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

#include "Net/UnrealNetwork.h"


AAuraPlayerState::AAuraPlayerState()
{
	// 이 액터가 1초에 얼마나 많이 Replication 할지를 의미한다.
	// 모든 Replication 은 Net Update 마다 수행되는데 이 주기(Net Update Time)를 결정하는데 영향을 미친다.
	// 보통의 게임에서는 0.5초 정도로 설정하는데, 포트나이트같은 Lyra 프로젝트에서는 평균 100정도로 설정한다.
	// 즉, 아래의 의미는 1초에 100번 Replication 하길 바란다. 즉 서버가 가능한 정도에서 최대 0.01초마다 Replicate 하려는 의도이다.
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level(const int32 OldLevel)
{

}
