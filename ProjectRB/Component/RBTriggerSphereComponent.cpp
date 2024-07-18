// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RBTriggerSphereComponent.h"
#include "Interact/RBInteractBase.h"
#include "Physics/RBCollision.h"


URBTriggerSphereComponent::URBTriggerSphereComponent()
{
	// Collision 을 Trigger 전용으로 설정
	SetCollisionProfileName(CPROFILE_RBTrigger);
}

void URBTriggerSphereComponent::BeginPlay()
{
	Super::BeginPlay();

	// Begin Overlap 이벤트에 델리게이트 콜백 추가
	OnComponentBeginOverlap.AddDynamic(this, &URBTriggerSphereComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &URBTriggerSphereComponent::OnOverlapEnd);
}

// 오버랩되면 Interact 가 있는 경우, Default Interact 를 액터에게 표시한다.
void URBTriggerSphereComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (URBInteractBase* InteractBase = Cast<URBInteractBase>(GetOwner()->GetComponentByClass(URBInteractBase::StaticClass())))
	{
		InteractBase->ShowInteractToActor(OtherActor);
	}
}

// 오버랩이 끝나면 Interact 가 있는 경우, Default Interact 를 액터에게서 삭제한다.
void URBTriggerSphereComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (URBInteractBase* InteractBase = Cast<URBInteractBase>(GetOwner()->GetComponentByClass(URBInteractBase::StaticClass())))
	{
		InteractBase->RemoveInteractFromActor(OtherActor);
	}
}