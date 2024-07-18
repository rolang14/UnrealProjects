
#include "Component/CR_MoverComponent.h"
#include "Math/UnrealMathUtility.h"
#include "CR_MoverComponent.h"


UCR_MoverComponent::UCR_MoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCR_MoverComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	OriginLocation = OwnerActor->GetActorLocation();
}

void UCR_MoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = OwnerActor->GetActorLocation();
	FVector TargetLocation = OriginLocation;

	if (IsReadyToMove)
	{
		TargetLocation = OriginLocation + MoveTo;
	}
	else
	{
		UStaticMeshComponent *StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (StaticMeshComponent == nullptr || StaticMeshComponent->IsSimulatingPhysics())
		{
			return;
		}
	}

	float Speed = MoveTo.Length() / MoveTime;
	FVector MoveVector = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	OwnerActor->SetActorLocation(MoveVector);
}

void UCR_MoverComponent::PlayEffectSound()
{
	if (ActionSounds.Num() && !IsPlayingSounds)
	{
		IsPlayingSounds = true;
		for (USoundBase* Sound : ActionSounds)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, GetOwner()->GetActorLocation());
		}
	}
}
