#include "Component/CR_Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Physics/CRCollision.h"
#include "CR_TagDefine.h"
#include "CR_Grabber.h"

UCR_Grabber::UCR_Grabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}  

void UCR_Grabber::BeginPlay()
{
	Super::BeginPlay();

	SetPhysicsHandle();
}

void UCR_Grabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FRotator ComponentRotation = GetComponentRotation();
	// AActor* OwnerActor = GetOwner();
	// UE_LOG(LogTemp, Display, TEXT("[%s] Rotation : %s"), *OwnerActor->GetActorNameOrLabel(), *ComponentRotation.ToCompactString());

	// float TS = GetWorld()->TimeSeconds;
	// UE_LOG(LogTemp, Display, TEXT("[%s] TimeSeconds : %f"), *OwnerActor->GetActorNameOrLabel(), TS);

	if (GetPhysicsHandle() == nullptr)
	{
		return;
	}

	// 잡은 컴포넌트를 액터(Grabber Component) 앞에 놓기
	UPrimitiveComponent *GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation + FVector(0, 0, HoldHeight), GetComponentRotation());
	}
}

void UCR_Grabber::Grab()
{
	if (GetPhysicsHandle() == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult))
	{
		AActor *HitActor = HitResult.GetActor();
		HitActor->Tags.Add(TAG_GRABBING);
		// UE_LOG(LogTemp, Display, TEXT("%s is hit !!"), *HitActor->GetActorNameOrLabel());
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, GrabRadius, 26, FColor::Red, false);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 26, FColor::Green, false);
		// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 26, FColor::Blue, false);

		// 가끔 Physics 가 Sleep 상태라 잡히지 않을 경우가 있음. 이 때 Rigid Body를 깨워서 사용해야 함.
		UPrimitiveComponent* GrabbedComponent = HitResult.GetComponent();
		GrabbedComponent->WakeAllRigidBodies();

		// Physics Handle 이 특정 컴포넌트를 잡은 상태가 되도록 함.
		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(
			GrabbedComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation());
	}
}

void UCR_Grabber::Release()
{	
	if (GetPhysicsHandle() == nullptr)
	{
		return;
	}

	UPrimitiveComponent *GrabbedComponent = GetPhysicsHandle()->GetGrabbedComponent();
	if (GrabbedComponent != nullptr)
	{
		AActor* GrabbedActor = GrabbedComponent->GetOwner();
		GrabbedActor->Tags.Remove(TAG_GRABBING);

		GrabbedComponent->WakeAllRigidBodies();
		GetPhysicsHandle()->ReleaseComponent();
	}
}

bool UCR_Grabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);
	
	// FQuat::Identity 는 회전값이 없다는 것을 의미함.
    return GetWorld()->SweepSingleByChannel(OutHitResult, StartLocation, EndLocation, FQuat::Identity, TCHANNEL_CRGRABBER, FCollisionShape::MakeSphere(GrabRadius));
}
