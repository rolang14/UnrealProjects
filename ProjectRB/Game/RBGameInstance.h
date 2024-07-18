// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RBGameInstance.generated.h"


class ARBCharacterEnemy;
class ARBCharacterPlayer;

struct FRBEnemyEncounterData;

/**
 * 
 */
UCLASS()
class PROJECTRB_API URBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 배틀 인카운터 준비
	void ReadyForBattleEncounter(TArray<FName> InPlayerPartyInfo, TSoftObjectPtr<UDataTable> InEnemyPartyInfo);

	// 배틀 인카운터 준비 (데이터라던지, 소환 위치라던지, 트랜지션 이펙트 재생이라던지 등)
	void PlayBattleEncounterTransition();

	// 캐릭터, 적 소환을 위한 BP (TSubclassOf)
	// 이걸 애셋 매니저로 대체가 가능할듯?
	// DataAsset 를 캐릭터마다 작성하고, 애셋 매니저를 통해 필요한 곳에서 얻는 것이 조금 더 바람직해 보인다.
	TSubclassOf<ARBCharacterPlayer> GetPlayerCharacterBPByName(const FName CharacterName);
	TSubclassOf<ARBCharacterEnemy> GetEnemyCharacterBPByName(const FName CharacterName);


	
private:
	UPROPERTY(EditAnywhere, Category = "Transition")
	TSubclassOf<UUserWidget> BattleLevelTransitionUI;

	UPROPERTY(EditAnywhere, Category = "Character")
	TSoftObjectPtr<UDataTable> CharacterPlayerData;

	UPROPERTY(EditAnywhere, Category = "Character")
	TSoftObjectPtr<UDataTable> CharacterEnemyData;

#pragma region Battle Encounting Data

public:
	// 전투에 진입했을 당시의 플레이어 파티 및 적 파티에 대한 정보를 반환 (To GameMode)
	FORCEINLINE const TArray<FName>& GetPlayerPartyInfo() const { return PlayerPartyInfo; }
	FORCEINLINE const TArray<FRBEnemyEncounterData>& GetEnemyPartyInfo() const { return EnemyPartyInfo; }

public:
	// 전투 진입 시의 플레이어 및 적의 파티 정보를 저장
	TArray<FName> PlayerPartyInfo;
	TArray<FRBEnemyEncounterData> EnemyPartyInfo;

#pragma endregion

};
