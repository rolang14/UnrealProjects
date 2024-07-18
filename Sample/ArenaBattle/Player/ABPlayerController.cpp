// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "Player/ABSaveGame.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	// HUD Widget �� ���̱� ���� Class ã��
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

	// Player0 ���Կ� SaveGameInstance �� ������.
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		// �����ϸ� �����α׸� ����.
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

	// �������ڸ��� ��Ŀ���� View ������ ���� ��.
	// ���� �������� �� FInputModeGameOnly ��� ����ü��
	// ���� ��ǲ�� ���� �¾��� �ϱ� ���� ����ü��µ� �Ƹ� ������ �����ϸ鼭
	// �̰Ÿ� SetInputMode�� ���������ν� �ٷ� ���Ӹ����ǲ�� �Ǿ �׷� �� ����.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// �� �۾��� �������Ʈ�� ����
	// ã�� Class �� �ش� Widget ����
	//ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	//if (ABHUDWidget)
	//{
	//	// ����Ʈ�� ���� Widget ����
	//	ABHUDWidget->AddToViewport();
	//}

	// UGameplayStatics �� ����, ���Կ������� ������ �ε��� �� �ִ� ���.
	// ������ ������ �̸��� �÷��̾��� ID (�̱ۿ����� 0���� �����ȴ�) �� ����Ѵ�.
	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		// �ε忡 �����ߴٸ� �ϴ� �׳� RetryCount �� �÷� ����.
		SaveGameInstance->RetryCount++;
	}
	else
	{
		// �ε���� ���ߴٸ� ���°Ŵϱ� ���� ����� ��.
		SaveGameInstance = NewObject<UABSaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
