// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameInstance/PuzzlePlatformGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumOfPlayer;

	if (NumOfPlayer >= PlayerNumToStart)
	{
		if (UWorld* World = GetWorld())
		{
			UE_LOG(LogTemp, Display, TEXT("Player fulfilled. Wait for %f Secionds . . ."), DelayTimeForStart);
			World->GetTimerManager().SetTimer(SessionStartTimerHandle, this, &ALobbyGameMode::StartSession, 5.0f, false);
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumOfPlayer;
}

void ALobbyGameMode::StartSession()
{
	if (UWorld* World = GetWorld())
	{
		UE_LOG(LogTemp, Display, TEXT("Now Server Travel Start . . ."));

		UPuzzlePlatformGameInstance* PuzzleGameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
		if (PuzzleGameInstance == nullptr) return;

		PuzzleGameInstance->StartSession();
		SessionStartTimerHandle.Invalidate();
		bUseSeamlessTravel = true;
		World->ServerTravel(TEXT("/Game/PuzzlePlatform/Maps/PuzzleMap1?listen"));
	}
}
