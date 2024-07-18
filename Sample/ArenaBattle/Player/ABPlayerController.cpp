// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "Player/ABSaveGame.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	// HUD Widget 을 붙이기 위해 Class 찾기
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AABPlayerController::GameOver()
{
	K2_OnGameOvered();

	// Player0 슬롯에 SaveGameInstance 를 저장함.
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		// 실패하면 에러로그를 띄운다.
		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error !"));
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AABPlayerController::GameClear()
{
	K2_OnGameCleared();
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작하자마자 포커스가 View 안으로 들어가게 됨.
	// 왠진 몰겠지만 이 FInputModeGameOnly 라는 구조체가
	// 유저 인풋에 대한 셋업을 하기 위한 구조체라는데 아마 게임이 시작하면서
	// 이거를 SetInputMode에 세팅함으로써 바로 게임모드인풋이 되어서 그런 것 같음.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 이 작업을 블루프린트로 변경
	// 찾은 Class 로 해당 Widget 생성
	//ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	//if (ABHUDWidget)
	//{
	//	// 뷰포트에 만든 Widget 띄우기
	//	ABHUDWidget->AddToViewport();
	//}

	// UGameplayStatics 에 의해, 슬롯에서부터 게임을 로드할 수 있는 기능.
	// 저장할 파일의 이름과 플레이어의 ID (싱글에서는 0번이 배정된다) 를 사용한다.
	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		// 로드에 성공했다면 일단 그냥 RetryCount 를 올려 보자.
		SaveGameInstance->RetryCount++;
	}
	else
	{
		// 로드되지 못했다면 없는거니까 새로 만들어 줌.
		SaveGameInstance = NewObject<UABSaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
