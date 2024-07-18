// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

	FORCEINLINE class UBoxComponent* GetTrigger() const { return Trigger; }

protected:
	virtual void PostInitializeComponents() override;

protected:
	// 루트 컴포넌트 (트리거용)
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	// 자식 컴포넌트 (Static Mesh, Box)
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect;

	// 아이템을 주웠을 때 액터에게 해당 아이템을 넘겨주기 위함.
	// (어떤 아이템인지는 모르므로 ABItemData로 선언)
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item;

	// 다이나믹 델리게이트이므로 UFUNCTION 매크로 선언
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
