// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RBOnFieldGameMode.h"
#include "Game/RBGameInstance.h"
#include "PlayerState/RBPlayerState.h"
#include "Controller/RBPlayerController.h"
#include "Character/RBCharacterEnemy.h"
#include "RBOnFieldGameMode.h"


void ARBOnFieldGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}

void ARBOnFieldGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ARBOnFieldGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameMode Begin Play"));

	SpawnCharacter();
}

// 디폴트 파티에 따른 캐릭터를 지정 위치에 소환한다.
void ARBOnFieldGameMode::SpawnCharacter()
{
	// 플레이어
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr) return;
	PlayerController->UnPossess();

	// Spawn Default Party
	if (UWorld* World = GetWorld())
	{
		// Player State
		ARBPlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<ARBPlayerState>();
		if (PlayerState == nullptr) return;

		for (uint8 i = 0; i < DefaultPlayableParty.Num(); ++i)
		{
			ARBCharacterPlayer* SpawnCharacter = World->SpawnActor<ARBCharacterPlayer>(DefaultPlayableParty[i], DefaultSpawnLocation, DefaultSpawnRotation);
			if (SpawnCharacter == nullptr) continue;
			PlayerState->AddCharacterToParty(SpawnCharacter);

			// 첫 번쨰 캐릭터를 기본으로 Possess 한다.
			if (i == 0) PlayerController->Possess(SpawnCharacter);
		}
	}

	// 마지막으로 플레이어를 FIELD_NORMAL 상태로 둔다.
	ARBPlayerController* RBPlayerController = Cast<ARBPlayerController>(PlayerController);
	if (RBPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set to Field Mode"));
		RBPlayerController->SetCurrentPlayerMode(EPlayerMode::FIELD_NORMAL);
	}
}

// 컨트롤 캐릭터를 변경한다.
void ARBOnFieldGameMode::SwitchPlayerCharacter(ARBPlayerController* PlayerController, int32 ChangeCharacterIndex)
{
	UE_LOG(LogTemp, Display, TEXT("Trying to Switch Player Character to %dth Character."), ChangeCharacterIndex);

	// Player State 를 가져온다.
	if (ARBPlayerState* PlayerState = PlayerController->GetPlayerState<ARBPlayerState>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Got Player State!!"));

		// Player State 에서 n번째 캐릭터를 받아온다.
		APawn* ControlPawn = PlayerState->GetCharacterAtIndex(ChangeCharacterIndex);

		// Controller 에게 Possess 요쳥
		if (ControlPawn)
		{
			PlayerController->Possess(ControlPawn);
			UE_LOG(LogTemp, Display, TEXT("Changed to %s"), *ControlPawn->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("There is no Character at %dth of party."), ChangeCharacterIndex);
		}
	}

	// 그 후, 배열의 순서는 그대로지만 이동 대열 순서를 바꾼다.
	// 아직 여기에서 해야 할 지는 확실하지 않음.
}

#pragma region Battle Encounter

// 배틀 인카운터 조건 검사 후 진입
void ARBOnFieldGameMode::RequestFieldEncounter(AActor* InAttacker, AActor* InReceiver)
{
	// 1. ARBCharacterBase 파생 클래스인지 확인 (ARBCharacterBase만 전투 진입이 가능함)
	ARBCharacterBase* Attacker = Cast<ARBCharacterBase>(InAttacker);
	ARBCharacterBase* Receiver = Cast<ARBCharacterBase>(InReceiver);
	if (!Attacker || !Receiver) return;

	// 2. 누가 플레이어인지 확인
	bool IsPlayerAttacked = false;
	ARBPlayerController* PlayerController = Cast<ARBPlayerController>(Attacker->GetController());
	ARBCharacterEnemy* Enemy = Cast<ARBCharacterEnemy>(Receiver);
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<ARBPlayerController>(Receiver->GetController());
		Enemy = Cast<ARBCharacterEnemy>(Attacker);
		IsPlayerAttacked = true;
	}

	// 3. 플레이어 vs 적의 상태만 전투 진입으로 취급한다.
	if (PlayerController == nullptr || Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Start Battle. One of Encountered Character is no available."));
		return;
	}

	// 4. 플레이어 최종 파티 정보와 적 파티 정보로 StartBattle 을 호출한다.
	ARBPlayerState* PlayerState = PlayerController->GetPlayerState<ARBPlayerState>();
	if (PlayerState == nullptr) return;

	// 5. 플레이어가 공격자인지, 피격자인지 설정한다.
	PlayerState->SetPlayerBattleCondition(IsPlayerAttacked);

	// 위 조건을 만족하면 배틀 인카운터
	StartBattle(PlayerState, Enemy);
}

// 배틀 인카운터를 준비하고 시작한다.
void ARBOnFieldGameMode::StartBattle(ARBPlayerState* PlayerState, ARBCharacterEnemy* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Encounting Battle !!!"));

	if (PlayerState == nullptr || Enemy == nullptr) return;

	// PlayerState 에서 현재 플레이어 파티 정보를 취득한다.
	TArray<FName> PlayerParty, EnemyParty;
	for (auto PlayerCharacter : PlayerState->GetPlayerParty())
	{
		if (ARBCharacterBase* RBCharacter = Cast<ARBCharacterBase>(PlayerCharacter)) PlayerParty.Add(RBCharacter->GetCharacterName());
	}

	// Enemy 에게서 전투할 Enemy 목록을 가져온다.
	TSoftObjectPtr<UDataTable> EnemyPartyInfo = Enemy->GetEnemyPartyInfo();

	URBGameInstance* RBGameInstance = Cast<URBGameInstance>(GetGameInstance());
	if (RBGameInstance == nullptr) return;

	// 다음 레벨을 위해 이 정보를 인스턴스에게 넘기고, 배틀 인카운터를 준비시킨다.
	RBGameInstance->ReadyForBattleEncounter(PlayerParty, EnemyPartyInfo);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	// 배틀 인카운터에 들어가면 월드 타임을 느려지게 하고, 3초의 시간을 둔다.
	World->GetWorldSettings()->TimeDilation = FMath::FInterpTo(World->GetWorldSettings()->TimeDilation, 0.01f, 3.f, 1.f);
	World->GetTimerManager().SetTimer(TransitionTimerHandle, this, &ARBOnFieldGameMode::MoveLevel, 3.0f * World->GetWorldSettings()->TimeDilation, false);
}

#pragma endregion

// 레벨 이동 (심리스)
void ARBOnFieldGameMode::MoveLevel()
{
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(TEXT("/Game/ProjectRB/Levels/Tokyo"));
	}
}