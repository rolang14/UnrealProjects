// Copyright NHE

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	// Singleton Get
	static const UAuraAssetManager& Get();

protected:
	// 이 함수는 게임이 시작되면 게임에서 사용할 Asset을 불러오는 함수이다. 필수적인 로딩인만큼 굉장히 이른 시점에 호출된다.
	// 즉 여기서 Gameplay Tag 역시 초기화해도 된다.
	virtual void StartInitialLoading() override;
};
