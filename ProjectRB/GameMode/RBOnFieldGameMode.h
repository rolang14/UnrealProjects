// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Character/RBCharacterPlayer.h"
#include "RBOnFieldGameMode.generated.h"


class ARBPlayerController;
class ARBPlayerState;
class ARBCharacterEnemy;


/// <summary>
/// GameMode for OnField
/// 
/// In Charge of : 
///		1. Battle Encounter
///		2. Player Party Management
/// </summary>

UCLASS()
class PROJECTRB_API ARBOnFieldGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

#pragma region Battle Encounter

public:
	// Check for Battle Encounter
	void RequestFieldEncounter(AActor* InAttacker, AActor* InReceiver);

private:
	// 배틀 레벨에 진입
	void StartBattle(ARBPlayerState* PlayerState, ARBCharacterEnemy* Enemy);
	// 실질적인 레벨 이동
	void MoveLevel();

private:
	// 배틀 진입 타이머
	FTimerHandle TransitionTimerHandle;

#pragma endregion

#pragma region Control Character / Party

public:
	// 컨트롤할 캐릭터를 변경
	void SwitchPlayerCharacter(ARBPlayerController* PlayerController, int32 ChangeCharacterIndex);

private:
	void SpawnCharacter();

private:
	// 초기에 시작될 파티 구성
	UPROPERTY(EditAnywhere, Category = "Default Party")
	TArray<TSubclassOf<ARBCharacterPlayer>> DefaultPlayableParty;

	// 초기에 스폰될 파티 위치
	UPROPERTY(EditAnywhere, Category = "Default Party", Meta = (MakeEditWidget = "true"))
	FVector DefaultSpawnLocation;

	// 초기에 스폰될 파티 방향
	UPROPERTY(EditAnywhere, Category = "Default Party", Meta = (MakeEditWidget = "true"))
	FRotator DefaultSpawnRotation;

#pragma endregion



};
