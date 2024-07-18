// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AABPlayerController();

	// �������Ʈ ȣȯ �Լ� ���� : �������Ʈ���� �̺�Ʈ�� �ߵ���Ű�°�ó�� ����� �����ϱ� ���ؼ��� BlueprintImplementableEvent �����ڸ� �߰��� �־�� �Ѵ�.
	// DisplayName �� ������ �������Ʈ���� ������ �̸��ε�, �̸� CPP���� ������ �̸��� �״�� ������� �򰥸� �� �����Ƿ�
	// OnScoreChanged + Cpp �� CPP���� ����������� �ǹ��ϴ� ���̻���� �ٿ��� �����.
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))
	// K2 ���λ�� �������Ʈ�� ������ Kismet �̶�� �ϴ� ����� ��Ÿ���� ���λ���.
	// UFUNCTION ��ũ�η� ���ǵ� �̺�Ʈ �������Ʈ �Լ��� ��쿡�� �𸮾� �������� �ڵ����� �����ϹǷ�
	// �����θ� ������ �ʾƵ� �ȴ�.
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearedCpp"))
	void K2_OnGameCleared();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOveredCpp"))
	void K2_OnGameOvered();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount);



	void GameScoreChanged(int32 NewScore);
	void GameOver();
	void GameClear();
	
protected:
	virtual void BeginPlay() override;

// HUD Section
protected:
	// ������ ������ �� �ʿ��� HUD Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	// ������ ������ ���� ������ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;

// Save Game Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UABSaveGame> SaveGameInstance;
};
