// Copyright NHE


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"


void UAuraGameplayAbility::UpdateWarpTarget(const FVector& TargetLocation)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetCurrentActorInfo()->AvatarActor))
	{
		CombatInterface->SetFacingTarget(TargetLocation);
	}
}
