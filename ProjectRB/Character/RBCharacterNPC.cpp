// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBCharacterNPC.h"
#include "Component/RBTriggerSphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


ARBCharacterNPC::ARBCharacterNPC()
{
	InteractTriggerSphere = CreateDefaultSubobject<URBTriggerSphereComponent>(TEXT("Talk Trigger Sphere"));
	InteractTriggerSphere->SetupAttachment(GetCapsuleComponent());
	InteractTriggerSphere->SetSphereRadius(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 100.f);
}