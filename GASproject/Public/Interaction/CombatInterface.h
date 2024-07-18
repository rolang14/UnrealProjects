// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	// 캐릭터의 Level 을 반환
	virtual int32 GetCharacterLevel();
	// Projectile 발사를 위해 캐릭터 무기의 소켓 위치 반환
	virtual FVector GetCombatSocketLocation();
	virtual void SetFacingTarget(const FVector& TargetLocation) = 0;
	// 캐릭터 사망 수행
	virtual void PerformDeath() = 0;

	// 인터페이스에서 UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) 매크로는 블루프린트에서 인터페이스의 함수를 호출할 수 있게 하지만, virtual 함수로는 구현할 수 없다.
	// BlueprintImplementable = 블루프린트에서 구현, BlueprintCallable = 블루프린트에서 호출 가능
	// 즉, CPP에서 Override 할 수 없다는 뜻이다.
	// 
	// 반면에, UFUNCTION(BlueprintNativeEvent) 는 CPP 구현, Blueprint 호출이 가능하다.
	// BlueprintNativeEvent 는 블루프린트로 덮어쓸 수 있도록 내장되어있으며, CPP에 기본 내장 또한 구현되어있기 때문에 CPP 정의가 가능하다. 함수 뒤에 _Implementation 을 붙여 코드를 작성하면 된다.
	// 만약 Blueprint Override 함수를 찾지 못하면 _Implementation CPP 내장 함수를 호출한다.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
};
