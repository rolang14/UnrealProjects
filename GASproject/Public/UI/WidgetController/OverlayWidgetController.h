// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "OverlayWidgetController.generated.h"


class UAuraUserWidget;
struct FOnAttributeChangeData;

// 게임플레이 태그 처리를 위한 TableRow Structure 작성.
// 이 데이터를 통해 Gameplay Tag를 Widget에서 핸들링한다.
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	// 반응할 게임플레이 태그
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageGameplayTag = FGameplayTag();

	// 에 따라 출력될 메세지 (위젯에 보여줄 것이므로 FText로 만든다.)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText();

	// 표시할 위젯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	// 보여줄 이미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

// Attribute Broadcast를 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewAttribute);
// FUIWidgetRow 를 Broadcast 하기 위함
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
#pragma region For Attribute Delegate

public:
	// BlueprintAssignable 은 Dynamic Multicast 델리게이트 한정이며, 블루프린트에서도 이 델리게이트를 사용할 수 있게 된다. 
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnMaxManaChanged;

	// UI에 보낼 데이터 메세지 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

public:
	// For Initial Value Delegate Broadcast
	virtual void BroadcastInitialValues() override;

	// Bind Changed Delegate for Attributes while gameplay.
	virtual void BindDelegatesForDependencies() override;

#pragma endregion

protected:
	// 템플릿 함수 생성. 어떠한 DataTableRow 라도 반환하게 하기 위함.
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

protected:
	// Anywhere 해도 되지만, DT의 경우 인스턴스별로 달라지기를 원하지 않으므로 Default 로 했다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;


};

// DataTable 에서 GameplayTag 를 찾아 해당 Row를 반환한다. Row Name 을 우리가 GameplayTag와 동일하게 만들었기 때문에 이렇게 검색이 가능하다.
template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
