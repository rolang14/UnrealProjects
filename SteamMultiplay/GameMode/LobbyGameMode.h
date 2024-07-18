// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	void StartSession();

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	uint8 PlayerNumToStart = 3;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	float DelayTimeForStart = 5.0f;

	FTimerHandle SessionStartTimerHandle;
	uint8 NumOfPlayer = 0;
};
