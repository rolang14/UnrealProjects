// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RBPlayerState.generated.h"


class ARBCharacterPlayer;

/**
 * 플레이어의 상태 보유
 *		1. 플레이어 파티 현황
 *		2. 
 */
UCLASS()
class PROJECTRB_API ARBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ARBPlayerState();

	// 특정 캐릭터를 파티에 추가한다.
	// 폰에 대한 포인터는 어떻게 관리해야할지 조금 생각해 봐야 할듯.
	// 1. UI에서 파티 편성할 때 캐릭터 소환
	// 2. Battle 로 넘어갔을 때 PlayerParty를 참고하여 캐릭터 소환
	// 3. 각 GameMode 에서 Default 로 설정한 파티 캐릭터 소환 (?)
	//	  지금으로써는 어디서든 쓸 수 있는 고유한 데이터 테이블을 만들고, 거기에 RowName 을 캐릭터 이믕 혹은 고유 코드로 하여 끌고 옴이 어떠한지 생각중
	void AddCharacterToParty(APawn* Character);

	// 특정 인덱스의 파티 멤버를 불러온다.
	APawn* GetCharacterAtIndex(int32 InIndex);

	FORCEINLINE const TArray<APawn*> GetPlayerParty() const { return PlayerParty; }

	FORCEINLINE void SetPlayerBattleCondition(const uint8 InIsAttacked) { bIsAttacked = InIsAttacked; }
	FORCEINLINE const uint8 GetPlayerBattleCondition() const { return bIsAttacked; }

protected:
	// Copy Properties 는 Seamless Travel 인 경우에만 작동한다.
	virtual void CopyProperties(APlayerState* NewPlayerState) override;
	
protected:
	// 실제로 파티를 컨트롤하기 위한 배열
	UPROPERTY(Replicated)
	TArray<APawn*> PlayerParty;

	UPROPERTY(Replicated)
	uint8 bIsAttacked : 1;
};
