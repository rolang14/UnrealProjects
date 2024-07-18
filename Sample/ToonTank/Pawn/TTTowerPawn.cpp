// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn/TTTowerPawn.h"
#include "Pawn/TTTankPawn.h"
#include "Kismet/GameplayStatics.h"

ATTTowerPawn::ATTTowerPawn()
{
}

void ATTTowerPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (CheckPlayerInRange())
    {
        RotateTurret(TankPawn->GetActorLocation());
    }
}

void ATTTowerPawn::BeginPlay()
{
    Super::BeginPlay();

    TankPawn = CastChecked<ATTTankPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    // DrawDebugSphere(GetWorld(), GetActorLocation(), FireRange, 12, FColor::Blue, true);

    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATTTowerPawn::CheckFireCondition, 2.f, true);
}

void ATTTowerPawn::CheckFireCondition()
{
    if (CheckPlayerInRange())
    {
        Fire();
    }
}

bool ATTTowerPawn::CheckPlayerInRange() const
{
    if (TankPawn && !TankPawn->IsPlayerDead)
    {
        if (GetPlayerDist() <= FireRange)
        {
            return true;
        }
    }

    return false;
}

const float ATTTowerPawn::GetPlayerDist() const
{
    if (TankPawn)
    {
        return FVector::Distance(TankPawn->GetActorLocation(), GetActorLocation());
    }

    return TNumericLimits<float>::Max();
}

void ATTTowerPawn::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}