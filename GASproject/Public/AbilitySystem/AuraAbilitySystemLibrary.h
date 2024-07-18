// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"


class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibraryWidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibraryWidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	// 레벨과 Class 에 따른 Attribute Initialize
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* InAbilitySystemComponent, const ECharacterClass InCharacterClass, const float InLevel);

	// Enemy 용 Ability 부여 함수
	static void GiveStarupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* InAbilitySystemComponent);

	// CharacterClassInfo 가 필요한 경우를 위한 함수
	static const UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
};
