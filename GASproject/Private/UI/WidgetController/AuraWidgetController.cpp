// Copyright NHE


#include "UI/WidgetController/AuraWidgetController.h"


// 컨트롤러에 데이터를 묶어놓는다.
void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	// do nothing.
}

void UAuraWidgetController::BindDelegatesForDependencies()
{
	// do nothing.
}
