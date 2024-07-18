// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TTPlayerController.h"


void ATTPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 이유는 모르겠는데 이걸 넣으니까 갑자기 포탄이 겁나 안나감
    FInputModeGameOnly GameOnlyInputMode;
    GameOnlyInputMode.SetConsumeCaptureMouseDown(false);
    SetInputMode(GameOnlyInputMode);
}

void ATTPlayerController::SetPlayerControllerEnabled(const bool bPlayerControllerEnabled)
{
    if (bPlayerControllerEnabled) GetPawn()->EnableInput(this);
    else GetPawn()->DisableInput(this);
    bShowMouseCursor = bPlayerControllerEnabled;
}