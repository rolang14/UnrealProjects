// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"


class UWidgetComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void BeginPlay() override;

#pragma region Highlight Enemy Interface

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

#pragma endregion

#pragma region Combat Interface

	virtual int32 GetCharacterLevel() override;

protected:
	virtual void PerformDeath() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DeathLifeSpan = 3.f;
	
#pragma endregion

#pragma region For GAS & Abilities

protected:
	// 어빌리티 셋업
	virtual void SetupAbilityInfo() override;

	// 기본 어트리뷰트 세팅 (능력치)
	virtual void InitializeDefaultAttributes() const override;

	// 게임플레이 태그 적용/제거 델리게이트 콜백 함수
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount);

	virtual void AddCharacterAbilities() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	uint8 bIsHitReacting : 1;

protected:
	// 적 레벨
	// Enemy 의 경우, AI가 컨트롤하므로 서버에서만 실행되며, 그렇기 때문에 Replicate 할 필요가 없다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Info")
	int32 Level = 1;

	// 적의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Info")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	

#pragma endregion

#pragma region For Enemy Widget

private:
	void SetupHealthWidgetComponent();

public:
	// 적 체력 UI에 Attribute를 반영하기 위함
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChanged OnMaxHealthChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;

#pragma endregion

#pragma region Character Movement

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float BaseWalkSpeed = 250.f;

#pragma endregion



};
