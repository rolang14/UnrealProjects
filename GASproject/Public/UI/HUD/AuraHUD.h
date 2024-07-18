// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

class UAuraUserWidget;
class UOverlayWidgetController;

class UAttributeMenuWidgetController;


struct FWidgetControllerParams;


/**
 * HUD를 관리하는 클래스
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
public:	
	// 이제 여기에, 데이터를 관리하는 컨트롤러를 추가로 넣고 싶다.
	// 따라서 AuraWidgetController 를 상속한 OverlayWidgetController 를 추가한다.
	// 컨트롤러는 데이터를 관리하고 있는만큼, 싱글톤처럼 사용하고 싶다. 그래서 아래와 같이 게터/세터를 정의한다.
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	// AttributeMenu Widget Controller 추가
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	// OverlayWidget 을 위한 Controller. (ViewModel)
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
