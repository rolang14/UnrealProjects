// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;


/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();

#pragma region Combat Interface

	virtual int32 GetCharacterLevel() override;

#pragma endregion


	// Possessed By 는 서버에서만! 호출된다.
	virtual void PossessedBy(AController* NewController) override;
	// 반대로 OnRep 는 클라이언트에서만! 호출된다.
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;

private:
	// 어빌리티 정보를 세팅한다.
	virtual void SetupAbilityInfo() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
};
