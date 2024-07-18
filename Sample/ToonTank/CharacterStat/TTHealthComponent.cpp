// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/TTHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/TTGameMode.h"


UTTHealthComponent::UTTHealthComponent()
{
	
}

// Called when the game starts
void UTTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTTHealthComponent::TakeDamage);
	
	ToonTankGameMode = Cast<ATTGameMode>(UGameplayStatics::GetGameMode(this));
	// check(ToonTankGameMode);
}

// 순서대로 대미지를 입을 액터, 대미지 양, 대미지 타입(뭐.. 독뎀 감전뎀 트루뎀 그런거), 대미지를 가한 액터(폰)을 조종하는 컨트롤러, 대미지를 가한 액터 : 여기서는 발자체 자신이 될것)
void UTTHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;
	CurHealth -= Damage;

	if (CurHealth <= 0)
	{
		ToonTankGameMode->SetActorDead(DamagedActor);
	}
}