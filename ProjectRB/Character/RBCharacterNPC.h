// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RBCharacterAI.h"
#include "RBCharacterNPC.generated.h"

class UCameraComponent;
class URBTriggerSphereComponent;

/**
 * NPC 캐릭터
 */
UCLASS()
class PROJECTRB_API ARBCharacterNPC : public ARBCharacterAI
{
	GENERATED_BODY()
	
public:
	ARBCharacterNPC();
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Interact Trigger", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URBTriggerSphereComponent> InteractTriggerSphere;
};
