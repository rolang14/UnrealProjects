// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/PlatformTrigger.h"
#include "Props/MovingPlatform.h"
#include "Components/BoxComponent.h"


// Sets default values
APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	if (!ensure(TriggerBox != nullptr)) return;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	if (!ensure(PlatformMesh != nullptr)) return;

	RootComponent = TriggerBox;
	PlatformMesh->SetupAttachment(TriggerBox);

	PlayerOnTrigger = 0;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::Activate);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::Deactivate);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	PlayerOnTrigger++;
	if (PlayerOnTrigger == 1)
	{
		UE_LOG(LogTemp, Display, TEXT("%s is Activated."), *GetName());
		for (AMovingPlatform* Platform : TargetPlatformList)
		{
			Platform->AddActivatedTrigger();
		}
	}
}

void APlatformTrigger::Deactivate(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerOnTrigger == 1)
	{
		UE_LOG(LogTemp, Display, TEXT("%s is Deactivated."), *GetName());
		for (AMovingPlatform* Platform : TargetPlatformList)
		{
			Platform->RemoveActivatedTrigger();
		}
	}

	PlayerOnTrigger--;
}

