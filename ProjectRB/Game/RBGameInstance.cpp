// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RBGameInstance.h"

#include "Character/RBCharacterBase.h"
#include "UI/RBFieldToBattleTransitionWidget.h"

#include "Data/RBCharacterPlayerData.h"
#include "Data/RBCharacterEnemyData.h"
#include "Data/RBEnemyEncounterData.h"


// 전투 진입을 위해서 다음 전투 레벨에 넘겨줄 데이터를 저장한다.
void URBGameInstance::ReadyForBattleEncounter(TArray<FName> InPlayerPartyInfo, TSoftObjectPtr<UDataTable> InEnemyPartyInfo)
{
	// 플레이어 파티 데이터를 복사한다.
	PlayerPartyInfo = InPlayerPartyInfo;

	// 적 파티 데이터 로드
	if (InEnemyPartyInfo.IsPending())
	{
		InEnemyPartyInfo.LoadSynchronous();
	}

	// 적 파티 데이터를 복사하여 저장한다.
	for (auto& it : InEnemyPartyInfo->GetRowMap())
	{
		//FRBEnemyEncounterData* EED = CastChecked<FRBEnemyEncounterData>(it.Value);
		FRBEnemyEncounterData* EED = (FRBEnemyEncounterData*)(it.Value);
		EnemyPartyInfo.Emplace(FRBEnemyEncounterData(EED->Type, EED->EnemyName, EED->Level));
	}

	PlayBattleEncounterTransition();
}

void URBGameInstance::PlayBattleEncounterTransition()
{
	// GameInstance 에서는 Tick을 사용할 수가 없어서 Lerp와 같은 기능을 쓸 수가 없다...

	// 전투 진입 트랜지션 재생
	URBFieldToBattleTransitionWidget* FBTransitionWidget = CreateWidget<URBFieldToBattleTransitionWidget>(this, BattleLevelTransitionUI);
	if (FBTransitionWidget == nullptr) return;

	FBTransitionWidget->AddToViewport();
	FBTransitionWidget->PlayTransition();
}

// 인카운터 시에 플레이어 스테이트에 만난 적 정보도 같이 넣으면 되는거 아닌가?
// 그리고 이렇게 게임인스턴스에서 가져오면 되는거 아님?
// 근데 그렇게 하는건 좀 깔끔하지가 않은것같다.. 전투 진입을 안하더라도 계속 그 데이터를 들고 다니는게 불필요함
TSubclassOf<ARBCharacterPlayer> URBGameInstance::GetPlayerCharacterBPByName(const FName CharacterName)
{
	if (CharacterPlayerData.IsPending())
	{
		CharacterPlayerData.LoadSynchronous();
	}

	// Row Name 0번째가 캐릭터 이름(Key)과 동일한 Row를 찾아 BP 정보를 반환한다.
	FString Result;
	FRBCharacterPlayerData* CharacterData = CharacterPlayerData->FindRow<FRBCharacterPlayerData>(CharacterName, Result);
	if (CharacterData == nullptr) return nullptr;

	return CharacterData->CharacterBP;
}

TSubclassOf<ARBCharacterEnemy> URBGameInstance::GetEnemyCharacterBPByName(const FName CharacterName)
{
	if (CharacterEnemyData.IsPending())
	{
		CharacterEnemyData.LoadSynchronous();
	}

	// Row Name 0번째가 캐릭터 이름(Key)과 동일한 Row를 찾아 BP 정보를 반환한다.
	FString Result;
	FRBCharacterEnemyData* CharacterData = CharacterEnemyData->FindRow<FRBCharacterEnemyData>(CharacterName, Result);
	if (CharacterData == nullptr) return nullptr;

	return CharacterData->CharacterBP;
}
