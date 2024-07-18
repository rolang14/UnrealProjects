// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/CR_TriggerComponent.h"
#include "CR_MeshChanger.h"
#include "CR_MoverComponent.h"
#include "CR_TagDefine.h"
#include "Interface/EffectSoundInterface.h"


void UCR_TriggerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCR_TriggerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    check(!UnlockTag.IsNone() && !(UnlockTag == NAME_None));

    // Release 되었을 때 트리거 발동
    AActor *OverlappingActor = FindOverlappedActorWithTag(UnlockTag);
    if (OverlappingActor == nullptr || OverlappingActor->ActorHasTag(TAG_GRABBING))
    {
        SetMoverMove(false);
        return;
    }

    // 트리거되어 변경되어야 하는 메시
    SetMeshChange(OverlappingActor);

    // 트리거의 키일때 정해진 위치로 이동해야 하는 경우
    // 오버랩되는 액터 본인이 Mover를 갖고 있으면, 본인을 옮긴다.
    UCR_MoverComponent *OverlappingMover = OverlappingActor->FindComponentByClass<UCR_MoverComponent>();
    if (OverlappingMover && !OverlappingActor->ActorHasTag(TAG_MOVING))
    {
        UPrimitiveComponent *PComponent = Cast<UPrimitiveComponent>(OverlappingActor->GetRootComponent());
        if (PComponent)
        {
            PComponent->SetSimulatePhysics(false);
            PComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        }

        FVector TargetLocation = GetComponentLocation() + FVector(0, 0, -1 * GetScaledBoxExtent().Z);
        FVector MoveLocation = TargetLocation - OverlappingActor->GetActorLocation();
        float OverlappingActorMoveTime = 3.0f;

        // 가고일 석상 위치 설정용
        OverlappingActor->Tags.Add(TAG_MOVING);
        OverlappingActor->SetActorRotation(GetOwner()->GetActorRotation() - FRotator(0, 90, 0));
        OverlappingMover->SetOriginLocation(OverlappingActor->GetActorLocation());
        OverlappingMover->SetMoveTo(MoveLocation);
        OverlappingMover->SetMoveTime(OverlappingActorMoveTime);
        OverlappingMover->SetReadyToMove(true);

        FTimerHandle MoveTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, FTimerDelegate::CreateLambda(
            [&]()
            {
                SetMoverMove(true);
            }
        ), OverlappingActorMoveTime, false);
    }
    else
    {
        SetMoverMove(true);
    }
}

void UCR_TriggerComponent::SetChangeComponent(UActorComponent *NewChangeComponent)
{
    ChangeComponent = NewChangeComponent;
}

void UCR_TriggerComponent::SetMeshChange(AActor *InOverlappingActor) const
{
    if (ChangeComponent == nullptr) return;
    UCR_MeshChanger *MeshChanger = Cast<UCR_MeshChanger>(ChangeComponent);
    if (MeshChanger)
    {
        MeshChanger->ChangeMesh();
        GetWorld()->DestroyActor(InOverlappingActor);
    }
}

void UCR_TriggerComponent::SetMoverMove(const bool IsReadyToMove) const
{
    if (ChangeComponent == nullptr) return;
    UCR_MoverComponent *Mover = Cast<UCR_MoverComponent>(ChangeComponent);
    if (Mover)
    {
        Mover->SetReadyToMove(IsReadyToMove);

        if (!IsReadyToMove) return;
        if (IEffectSoundInterface* EffectSoundInterface = Cast<IEffectSoundInterface>(Mover))
        {
            EffectSoundInterface->PlayEffectSound();
        }
    }
}