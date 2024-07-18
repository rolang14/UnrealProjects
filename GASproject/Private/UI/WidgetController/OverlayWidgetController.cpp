// Copyright NHE


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"


// 최초 값 브로드캐스트
void UOverlayWidgetController::BroadcastInitialValues()
{
	// 현재는 AuraAttributeSet 클래스만 있기 때문에 CastChecked 로 Assert를 사용해도 된다.
	// 대신, 반드시 초기화 된 시점 이후에 BroadCast 해야 한다.
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

// 그 이후 값이 변화할 때마다 브로드캐스트하기 위한 델리게이트 바인딩
void UOverlayWidgetController::BindDelegatesForDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// ASC에서는 기본적으로 모든 AttributeValue에 대한 ChangeDelegate 를 제공한다.
	// 따라서 여기에 바인딩하면 Attribute의 변화를 감지할 수 있다.
	// 모두 짧은 함수이므로 델리게이트 콜백을 람다식으로 대체한다.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangedHealth)
		{
			OnHealthChanged.Broadcast(ChangedHealth.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangedMaxHealth)
		{
			OnMaxHealthChanged.Broadcast(ChangedMaxHealth.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangedMana)
		{
			OnManaChanged.Broadcast(ChangedMana.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangedMaxMana)
		{
			OnMaxManaChanged.Broadcast(ChangedMaxMana.NewValue);
		}
	);

	// "AssetTag" 델리게이트 람다 Function 바인딩
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTagsChanged.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// FGameplayTag::MatchesTag(const FGameplayTag&) : 특정 카테고리(부모)의 태그까지 일치하는지 검사한다.
				// 태그가 만약 "A.1" 이라면, "A" 를 검사했을 때 true 가 나오고, 태그가 "A" 라면 "A.1" 을 검사했을 때 false 가 나온다.
				// 이 검사를 위해, GameplayTag를 가져온다. 만약 config 에 해당 Tag가 없다면 가져오지 못한다.
				// 만약 "Message" 태그는 없고, "Message.Health", "Message.Mana" 와 같이 자식 태그만 있더라도 Message 태그가 존재하는 것으로 친다.
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					// 즉, Tag가 메세지 태그라면 아래를 수행한다.

					// 그러나 람다 함수에서는 전달받은 멤버나 Global 멤버 이외에는 알지 못한다. 즉 헤더에 선언한 아래의 템플릿 함수도 모른다.
					// []에다가 this 를 적어 주어, 이 객체를 레퍼런싱 하게 해야 한다.
					FUIWidgetRow* DataRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*DataRow);
				}


			}
		}
	);
}