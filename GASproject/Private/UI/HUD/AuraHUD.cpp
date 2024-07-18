// Copyright NHE


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"


void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// checkf : 조건을 체크하고, 에러 시에는 TEXT를 출력한다.
	checkf(OverlayWidgetClass, TEXT("OverlayWidget Class is not set. Please fill out OverlayWidgetClass at BP_AuraHUD."));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetController Class is not set. Please fill out OverlayWidgetControllerClass at BP_AuraHUD."));

	// 우선 위젯을 만들고
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);

	// 붙여 줄 위젯 컨트롤러를 가져온다.
	// 싱글톤의 Getter 는 그와 동시에 Constructor 역할도 하므로 Params만 만들어서 Get 하면 된다.
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// 이 시점에서 Widget이 Controller를 알게 된다.
	OverlayWidget->SetWidgetController(WidgetController);

	// 때문에 이 때 Broadcast를 하게 되면 Widget이 이를 알 수 있다.
	WidgetController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	// 싱글톤처럼 없으면 만들고 있으면 반환한다. Controller 는 한번만 생성되게 하기 위함이다.
	// 본인은 Private 여야 하겠지?
	if (OverlayWidgetController == nullptr)
	{
		// UObject를 이렇게 동적으로 만들 경우에는 TSubclassOf가 필요하다.
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		// Params 를 넣었다면 이제 필요한 데이터는 모두 갖춘 것이므로 ASC를 사용하여 AS 멤버에 델리게이트를 바인딩할 수 있다.
		OverlayWidgetController->BindDelegatesForDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AttributeMenuWidgetController->BindDelegatesForDependencies();
	}
	return AttributeMenuWidgetController;
}
