#include "Component/CR_MeshChanger.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"


UCR_MeshChanger::UCR_MeshChanger()
{
	ComponentNames.Emplace(TEXT("Lock"));
	ComponentNames.Emplace(TEXT("SM_Cell_Door"));
	ComponentNames.Emplace(TEXT("PhysicsConstraint"));
}


// Called when the game starts
void UCR_MeshChanger::BeginPlay()
{
	Super::BeginPlay();

	// 이렇게 액터에 속한 컴포넌트 중 원하는 것을 이름으로 직접 가져올 수도 있다.
	// LockStaticMeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("Lock")));
	// if (LockStaticMeshComponent == nullptr)
	// {
	// 	return;
	// }

	// DoorStaticMeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("SM_Cell_Door")));
	// if (DoorStaticMeshComponent == nullptr)
	// {
	// 	return;
	// }

	// PhysicsConstraintComponent = Cast<UPhysicsConstraintComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("PhysicsConstraint")));
	// if (PhysicsConstraintComponent == nullptr)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Can't find Physics Constraint Component."));
	// 	return;
	// }
	
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetOwner()->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
	for (UStaticMeshComponent* InStaticMeshComponent : StaticMeshComponents)
	{
		if (InStaticMeshComponent->GetName().Equals(ComponentNames[(uint8)EComponentNames::Lock], ESearchCase::Type::CaseSensitive))
		{
			LockStaticMeshComponent = InStaticMeshComponent;
			// UE_LOG(LogTemp, Display, TEXT("%s"), *ComponentNames[(uint8)EComponentNames::Lock]);
		}

		if (InStaticMeshComponent->GetName().Equals(ComponentNames[(uint8)EComponentNames::Door], ESearchCase::Type::CaseSensitive))
		{
			DoorStaticMeshComponent = InStaticMeshComponent;
			// UE_LOG(LogTemp, Display, TEXT("%s"), *ComponentNames[(uint8)EComponentNames::Door]);
		}
	}

	TArray<UPhysicsConstraintComponent*> PhysicsConstraintComponents;
	GetOwner()->GetComponents<UPhysicsConstraintComponent>(PhysicsConstraintComponents);
	for (UPhysicsConstraintComponent* InPhysicsConstraintComponent : PhysicsConstraintComponents)
	{
		if (InPhysicsConstraintComponent->GetName().Equals(ComponentNames[(uint8)EComponentNames::PhysicsConstraint], ESearchCase::Type::CaseSensitive))
		{
			PhysicsConstraintComponent = InPhysicsConstraintComponent;
			// UE_LOG(LogTemp, Display, TEXT("%s"), *ComponentNames[(uint8)EComponentNames::PhysicsConstraint]);
		}
	}
}

void UCR_MeshChanger::ChangeMesh()
{
	MeshChanged = true;

	// AStaticMeshActor* OwnerStaticMeshActor = Cast<AStaticMeshActor>(GetOwner());
	// if (OwnerStaticMeshActor == nullptr)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Can't find OwnerStaticMeshActor."));
	// 	return;
	// }

	if (ActionSounds.Num())
	{
		for (USoundBase* Sound : ActionSounds)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, GetOwner()->GetActorLocation());
		}
	}

	// 자물쇠 Physics 처리
	// Movable 까지 안하면 Physics 키더라도 움직이질 않는다.
	// 혹시 땅을 뚫고 지나가면 Ridgid Body 혹은 콜리전 확인하자
	LockStaticMeshComponent->SetMobility(EComponentMobility::Type::Movable);
	if (LockStaticMeshComponent->SetStaticMesh(TargetStaticMesh))
	{
		LockStaticMeshComponent->SetSimulatePhysics(true);
		LockStaticMeshComponent->WakeAllRigidBodies();
	}

	// 문 Physics 처리 (열림 피직스 상태로)
	DoorStaticMeshComponent->SetMobility(EComponentMobility::Type::Movable);
	DoorStaticMeshComponent->SetSimulatePhysics(true);
	DoorStaticMeshComponent->WakeAllRigidBodies();
	// PhysicsConstraintComponent->ComponentName1.ComponentName = *DoorStaticMeshComponent->GetName();
	PhysicsConstraintComponent->SetConstrainedComponents(DoorStaticMeshComponent, NAME_None, nullptr, NAME_None);
}