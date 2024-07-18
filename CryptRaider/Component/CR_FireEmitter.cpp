// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CR_FireEmitter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


void UCR_FireEmitter::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    SetFireOn(FindOverlappedActorWithTag(InflamableTag));
}

void UCR_FireEmitter::SetFireOn(AActor* InflamableActor)
{
    if (InflamableActor)
    {
        if (BurnSound && !bIsBurnedOnce)
        {
            UGameplayStatics::PlaySoundAtLocation(this, BurnSound, GetOwner()->GetActorLocation());
            bIsBurnedOnce = true;
        }

        UParticleSystemComponent* FireParticle = Cast<UParticleSystemComponent>(InflamableActor->GetDefaultSubobjectByName(TEXT("EmitFireParticle")));
        check(FireParticle);
        FireParticle->SetVisibility(true);

    }
}