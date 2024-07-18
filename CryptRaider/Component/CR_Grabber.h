// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CR_Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UCR_Grabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCR_Grabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();

private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 150.0f;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 50.0f;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 50.0f;

	UPROPERTY(EditAnywhere)
	float HoldHeight = 0.0f;

private:
	FORCEINLINE UPhysicsHandleComponent* const GetPhysicsHandle() const { if (PhysicsHandle == nullptr) { UE_LOG(LogTemp, Error, TEXT("Physics Handle is invalid.")); } return PhysicsHandle; }
	void SetPhysicsHandle() { PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); }
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	UPhysicsHandleComponent* PhysicsHandle;
};
