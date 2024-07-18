// Copyright NHE


#include "AbilitySystem/Data/AttributeInfo.h"


FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& InAttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		// Tag가 일치하면 찾은것
		if (AttributeInfo.AttributeTag.MatchesTagExact(InAttributeTag))
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		// GetNameSafe(this) 는 이 클래스의 이름을 가져온다.
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *InAttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
