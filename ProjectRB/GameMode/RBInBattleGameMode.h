// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RBInBattleGameMode.generated.h"


class ARBCharacterPlayer;

/// <summary>
/// GameMode for Battle
/// 
/// In Charge of : 
///		1. Turn-Based Battle
///		2. Real-time Battle (Boost Mode)
/// </summary>
UCLASS()
class PROJECTRB_API ARBInBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// 플레이어 파티 소환 및 초기화
	virtual void GenericPlayerInitialization(AController* Controller) override;

protected:
	virtual void BeginPlay() override;

	void SpawnPlayer(AController* Controller);
	void SpawnEnemy();

private:
	// 파티에 있는 캐릭터 소환을 위해
	// FString (캐릭터 고유 이름 혹은 코드) 과 매칭되는 BP를 사용한다.
	//UPROPERTY(EditAnywhere, Category = "Battle Character")
	//TMap<FName, TSubclassOf<ARBCharacterPlayer>> PlayerBattleCharacterClass;

	// 초기에 스폰될 플레이어 파티 위치 (각각의 캐릭터 순서와 매칭)
	UPROPERTY(EditAnywhere, Category = "Battle Character", Meta = (MakeEditWidget = "true"))
	TArray<FVector> PlayerSpawnLocations;

	// 초기에 스폰될 플레이어 파티 방향 (각각의 캐릭터 순서와 매칭)
	UPROPERTY(EditAnywhere, Category = "Battle Character", Meta = (MakeEditWidget = "true"))
	TArray<FRotator> PlayerSpawnRotations;

	// 초기에 스폰될 적 파티 위치 (각각의 캐릭터 순서와 매칭)
	UPROPERTY(EditAnywhere, Category = "Battle Character", Meta = (MakeEditWidget = "true"))
	TArray<FVector> EnemySpawnLocations;

	// 초기에 스폰될 적 파티 방향 (각각의 캐릭터 순서와 매칭)
	UPROPERTY(EditAnywhere, Category = "Battle Character", Meta = (MakeEditWidget = "true"))
	TArray<FRotator> EnemySpawnRotations;
};
