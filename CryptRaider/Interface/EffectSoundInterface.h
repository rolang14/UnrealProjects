#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EffectSoundInterface.generated.h"


UINTERFACE(MinimalAPI)
class UEffectSoundInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYPTRAIDER_API IEffectSoundInterface
{
	GENERATED_BODY()

public:
	virtual void PlayEffectSound() = 0;
};
