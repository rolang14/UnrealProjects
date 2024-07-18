// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CR_InflamableActionComponent.h"


void UCR_InflamableActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FireActor = FindOverlappedActorBurning();
    if (FireActor && !bIsOnFire)
    {
        DestroyWithFire(DeltaTime);
    }
}

void UCR_InflamableActionComponent::DestroyWithFire(const float DeltaTime)
{
    bIsOnFire = true;

    UE_LOG(LogTemp, Warning, TEXT("Destroy"));

    check(BurnMaterial);
    UStaticMeshComponent *PlasterStaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    UMaterialInstanceDynamic *PlasterBurnDynamicMaterial = UMaterialInstanceDynamic::Create(BurnMaterial, GetOwner());
    PlasterStaticMeshComponent->SetMaterial(0, PlasterBurnDynamicMaterial);

    OnStartDissolve.Broadcast(PlasterBurnDynamicMaterial);
}