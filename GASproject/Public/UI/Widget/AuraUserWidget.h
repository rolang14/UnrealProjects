// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"


struct FUIWidgetRow;

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region For Widget Controller

public:
	// 블루프린트에서 호출 가능하게 만든다.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetWidgetController(UObject* InWidgetController) { WidgetController = InWidgetController; OnWidgetControllerSet(); }

protected:
	// 블루프린트에서 처리할 이벤트. WidgetController를 Set 하면 호출된다.
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

public:
	// 모든 AuraUserWidget 을 상속하는 위젯은 WidgetController 를 갖는다. 편리하게 사용하기 위해 UObject로 WidgetController 를 작성한다.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

#pragma endregion

	

	
};
