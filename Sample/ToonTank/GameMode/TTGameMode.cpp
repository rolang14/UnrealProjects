// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TTGameMode.h"
#include "Pawn/TTTankPawn.h"
#include "Pawn/TTTowerPawn.h"
#include "Controller/TTPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ATTGameMode::BeginPlay()
{
    Super::BeginPlay();

    NumOfTotalEnemy = GetTowerEnemyNumber();

    GameStart();

    TankPlayer = Cast<ATTTankPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
    TankPlayerController = Cast<ATTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (TankPlayerController)
    {
        TankPlayerController->SetPlayerControllerEnabled(false);

        FTimerDelegate StartDelayTimerDelegate = FTimerDelegate::CreateUObject(TankPlayerController, &ATTPlayerController::SetPlayerControllerEnabled, true);
        GetWorldTimerManager().SetTimer(StartDelayTimerHandle, StartDelayTimerDelegate, StartDelay, false);
    }
}

void ATTGameMode::SetActorDead(AActor* DeadActor)
{
    // if , else 로직에서 DeadActor 가 nullptr 인지 확인하는 것이나 다름 없으므로 구태여 확인할 필요는 없다.
    if (DeadActor == TankPlayer)
    {
        GameWon(false);
        TankPlayer->HandleDestruction();
        if (TankPlayerController) TankPlayerController->SetPlayerControllerEnabled(false);
    }
    else if (ATTTowerPawn* TowerPawn = Cast<ATTTowerPawn>(DeadActor))
    {
        if (--NumOfTotalEnemy <= 0) GameWon(true);
        TowerPawn->HandleDestruction();
    }
}

const int32 ATTGameMode::GetTowerEnemyNumber()
{
    TArray<AActor*> FoundEnemy;
    UGameplayStatics::GetAllActorsOfClass(this, ATTTowerPawn::StaticClass(), FoundEnemy);
    return FoundEnemy.Num();
}