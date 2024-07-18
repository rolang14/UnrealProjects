// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CR_MeshChanger.generated.h"


UENUM(BlueprintType)
enum class EComponentNames : uint8
{
	Lock = 0,
	Door,
	PhysicsConstraint
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UCR_MeshChanger : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCR_MeshChanger();

	void ChangeMesh();

	FORCEINLINE const bool GetMeshChanged() const { return MeshChanged; }

protected:
	virtual void BeginPlay() override;

protected:	
	UPROPERTY(EditAnywhere, Category = "Mesh Action")
	TObjectPtr<UStaticMesh> TargetStaticMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh Action")
	TArray<TObjectPtr<USoundBase>> ActionSounds;


private:
	UStaticMeshComponent* LockStaticMeshComponent;
	UStaticMeshComponent* DoorStaticMeshComponent;
	UPhysicsConstraintComponent* PhysicsConstraintComponent;

	TArray<FString> ComponentNames;

	bool MeshChanged = false;
};
