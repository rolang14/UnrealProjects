// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RBInBattleGameMode.h"
#include "Game/RBGameInstance.h"

#include "Controller/RBPlayerController.h"
#include "PlayerState/RBPlayerState.h"

#include "Character/RBCharacterPlayer.h"
#include "Character/RBCharacterEnemy.h"

#include "Data/RBEnemyEncounterData.h"


void ARBInBattleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("Battle Game Mode Start !!"));
}

void ARBInBattleGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);

	SpawnPlayer(Controller);
	SpawnEnemy();
}

// 인카운터 플레이어 파티 정보를 통해 플레이어 캐릭터들을 현재 레벨에 소환
void ARBInBattleGameMode::SpawnPlayer(AController* Controller)
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	ARBPlayerController* RBPlayerController = Cast<ARBPlayerController>(Controller);
	if (RBPlayerController == nullptr) return;

	ARBPlayerState* PlayerState = RBPlayerController->GetPlayerState<ARBPlayerState>();
	if (PlayerState == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Set to Battle Mode"));

	// 플레이어를 배틀 모드로 변경한다.
	RBPlayerController->SetCurrentPlayerMode(EPlayerMode::BATTLE_NORMAL);

	// 현재 파티에 있는 캐릭터들을 소환한다.
	const TArray<FName>& PlayerPartyInfo = GameInstance->GetPlayerPartyInfo();
	for (uint8 i = 0; i < PlayerPartyInfo.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character : %s is joined."), *PlayerPartyInfo[i].ToString());
		// 여기서 소환. PlayerStart 위치 정보를 가져올 수는 없나?
		ARBCharacterPlayer* SpawnedCharacter = GetWorld()->SpawnActor<ARBCharacterPlayer>(GameInstance->GetPlayerCharacterBPByName(PlayerPartyInfo[i]), PlayerSpawnLocations[i], PlayerSpawnRotations[i]);
		if (SpawnedCharacter == nullptr) continue;

		PlayerState->AddCharacterToParty(SpawnedCharacter);

		if (i == 0) RBPlayerController->Possess(SpawnedCharacter);
	}
}

// 인카운터 적 파티 정보를 통해 플레이어 캐릭터들을 현재 레벨에 소환
void ARBInBattleGameMode::SpawnEnemy()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	const TArray<FRBEnemyEncounterData>& EnemyPartyInfo = GameInstance->GetEnemyPartyInfo();
	for (uint8 i = 0; i < EnemyPartyInfo.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy : %s is joined."), *EnemyPartyInfo[i].EnemyName.ToString());

		// 소환
		ARBCharacterEnemy* SpawnedCharacter = GetWorld()->SpawnActor<ARBCharacterEnemy>(GameInstance->GetEnemyCharacterBPByName(EnemyPartyInfo[i].EnemyName), EnemySpawnLocations[i], EnemySpawnRotations[i]);
		if (SpawnedCharacter == nullptr) continue;
	}
}

void ARBInBattleGameMode::BeginPlay()
{

}