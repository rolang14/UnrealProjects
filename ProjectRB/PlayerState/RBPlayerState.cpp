// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/RBPlayerState.h"
#include "Character/RBCharacterPlayer.h"
#include "RBPlayerState.h"

#include "Net/UnrealNetwork.h"


const static int32 MAX_PARTY_NUM = 4;

ARBPlayerState::ARBPlayerState()
{
	PlayerParty = TArray<APawn*>();
}

//
void ARBPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	UE_LOG(LogTemp, Warning, TEXT("Copy Properties in New PlayerState"));
	if (ARBPlayerState* NPS = Cast<ARBPlayerState>(NewPlayerState))
	{
		// Something you have to copy
	}
}

void ARBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	UE_LOG(LogTemp, Warning, TEXT("GetLifetimeReplicatedProps"));

	DOREPLIFETIME(ARBPlayerState, PlayerParty);
	DOREPLIFETIME(ARBPlayerState, bIsAttacked);
}

// 파티에 캐릭터를 추가
void ARBPlayerState::AddCharacterToParty(APawn* Character)
{
	if (HasAuthority())
	{
		if (Character == nullptr) return;

		// 파티 인원 체크, 중복체크 후 해당 캐릭터를 파티 배열에 추가
		if (PlayerParty.Num() < MAX_PARTY_NUM && !PlayerParty.Contains(Character))
		{
			// 내부 파티 캐릭터 데이터 추가
			PlayerParty.Emplace(Character);
		}
	}
}

// 요청한 인덱스의 캐릭터를 반환
APawn* ARBPlayerState::GetCharacterAtIndex(int32 InIndex)
{
	// 파티 배열에 존재하는지 체크
	if (!PlayerParty.IsValidIndex(InIndex - 1) || PlayerParty[InIndex - 1] == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("There is no %dth Character in party."), InIndex);
		return nullptr;
	}

	UE_LOG(LogTemp, Display, TEXT("1st Character : %d"), PlayerParty[InIndex - 1]);
	return PlayerParty[InIndex - 1];
}