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
	// ��Ʈ ������Ʈ (Ʈ���ſ�)
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	// �ڽ� ������Ʈ (Static Mesh, Box)
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// ����Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect;

	// �������� �ֿ��� �� ���Ϳ��� �ش� �������� �Ѱ��ֱ� ����.
	// (� ������������ �𸣹Ƿ� ABItemData�� ����)
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item;

	// ���̳��� ��������Ʈ�̹Ƿ� UFUNCTION ��ũ�� ����
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
