// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CR_OverlapBoxComponentBase.h"
#include "Particles/ParticleSystemComponent.h"


UCR_OverlapBoxComponentBase::UCR_OverlapBoxComponentBase()
{
    PrimaryComponentTick.bCanEverTick = true;
}

AActor* UCR_OverlapBoxComponentBase::FindOverlappedActorWithTag(const FName &InActorTag) const
{
    TArray<AActor *> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    for (AActor *InActor : OverlappingActors)
    {
        if (InActor->ActorHasTag(InActorTag))
        {
            return InActor;
        }
    }

    return nullptr;
}

AActor* UCR_OverlapBoxComponentBase::FindOverlappedActorBurning() const
{
    TArray<AActor *> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    for (AActor *InActor : OverlappingActors)
    {
        UParticleSystemComponent* FireParticleComponent = Cast<UParticleSystemComponent>(InActor->GetDefaultSubobjectByName(TEXT("EmitFireParticle")));
        if (FireParticleComponent && FireParticleComponent->IsVisible())
        {
            return InActor;
        }
    }

    return nullptr;
}