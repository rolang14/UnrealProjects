// Copyright NHE


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (const auto& it : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(it.Key, it.Value());
	}
}

void UAttributeMenuWidgetController::BindDelegatesForDependencies()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	// 여기도 동일한 방법으로 할 수 있다.
	for (const auto& it : AS->TagsToAttributes)
	{
		// 모든 Attribute 의 ValueChangeDelegate 에
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(it.Value()).AddLambda(
			[this, it](const FOnAttributeChangeData& Data)
			{
				// 사실상 여기서 Data 는 델리게이트 시그니처라서 넣은것일 뿐이지, 이미 Map에서 모든 바뀐 정보를 얻어올 수 있기 때문에 (it.Value() 한 순간 데이터를 가져오기 때문)
				BroadcastAttributeInfo(it.Key, it.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& GameplayAttribute) const
{
	check(AttributeInfoData);
	// Attribute Data Asset 으로부터 게임플레이 태그를 통해 정보를 불러온다.
	FAuraAttributeInfo AttributeInfo = AttributeInfoData->FindAttributeInfoForTag(GameplayTag);
	// AS가 필요한 이유는 it.Value.Execute 가 실제로는 Retval Delegate 를 실행하므로 Attribute 를 가져오게 되고, 실제로 이는 Static Function 이다.
	// 따라서 여기서 Value 를 가져오려면 어떤 AS가 이를 정의하는지 Context 를 알아야 한다. (Static Function 은 인스턴스 없이 바로 호출되기 때문)
	AttributeInfo.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AuraAttributeInfoDelegate.Broadcast(AttributeInfo);
}
