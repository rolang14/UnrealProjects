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

	// 블루프린트 호환 함수 정의 : 블루프린트에서 이벤트를 발동시키는것처럼 기능을 수행하기 위해서는 BlueprintImplementableEvent 지정자를 추가해 주어야 한다.
	// DisplayName 은 실제로 블루프린트에서 보여질 이름인데, 이를 CPP에서 정의한 이름을 그대로 써버리면 헷갈릴 수 있으므로
	// OnScoreChanged + Cpp 로 CPP에서 만들어졌음을 의미하는 접미사까지 붙여서 만든다.
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))
	// K2 접두사는 블루프린트의 전신인 Kismet 이라고 하는 기능을 나타내는 접두사임.
	// UFUNCTION 매크로로 정의된 이벤트 블루프린트 함수의 경우에는 언리얼 엔진에서 자동으로 생산하므로
	// 구현부를 만들지 않아도 된다.
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
	// 위젯을 생성할 때 필요한 HUD 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	// 생성한 위젯을 담을 위젯의 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;

// Save Game Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UABSaveGame> SaveGameInstance;
};
