// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"


struct FGameplayAttribute;


// ** 델리게이트 정의를 위해서는 매개 변수의 사양을 알아야 하기 때문에 전방선언 할 수 없다.
// Attribute 별로 델리게이트를 만들지 않고, FAuraAttributeInfo 를 통해 통합적으로 Broadcast 하기 위함
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraAttributeInfoSignature, const FAuraAttributeInfo&, AuraAttributeInfo);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindDelegatesForDependencies() override;

private:
	void BroadcastAttributeInfo(const FGameplayTag& GameplayTag, const FGameplayAttribute& GameplayAttribute) const;

public:

#pragma region Attribute Info Delegate

	UPROPERTY(BlueprintAssignable, Category = "GAS|AttributeInfo")
	FAuraAttributeInfoSignature AuraAttributeInfoDelegate;

#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Information")
	TObjectPtr<UAttributeInfo> AttributeInfoData;

};
