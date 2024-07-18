// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"


// DECLARE_DELEGATE_OneParam(FOnServerFoundCompleteDelegate, const FName&/*Found Server Name*/)

class UMenuWidgetBase;
class UMainMenu;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	//FOnServerFoundCompleteDelegate OnServerFoundCompleteDelegate;
	
public:
	// 게임 인스턴스 생성자
	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);

	// 필요한 값 세팅용 함수
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadPauseMenu();

	// 서버를 찾고 세션을 표시하는 명령어
	virtual void SearchSession() override;

	// 호스트 중인 게임 서버에 참여하는 명령어
	UFUNCTION(Exec)
	virtual void JoinSession(const uint32 Index) override;

	// 실제로 게임을 시작하고, 더 이상 이 세션을 유효하지 않게 만든다.
	void StartSession();

	// 현재 방을 떠나기
	UFUNCTION()
	virtual void LeaveGame() override;

	// 게임 프로그램 종료
	UFUNCTION()
	virtual void QuitGame() override;

// Online Subsystem Secssion
private:
	virtual void StartHosting() override;

	IOnlineSessionPtr SessionInterfacePtr;
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// 이것을 Search 이후에도 사용할 것이므로 멤버로 선언을 해야 한다.
	// 별도의 UObject 로 생성하여 관리하지 않는 이유는, 그렇게 덩치를 키울 필요도 없고, 그렇게 할 필요도 없기 때문이다.
	// 때문에 이를 TSharedPtr 로 선언하여 참조를 카운팅당하게 함으로써 이 인스턴스가 살아있는 동안, 혹은 명시적으로 하지 않는 동안에는 GC당하지 않게 설정하고 (중간에 써야 하니까)
	// 애초에 TSharedRef 를 사용하도록 고려가 되어 있기 때문에 이와 같이 선언한다.
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 Join 후 결과 콜백
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionResult);

	// 네트워크 처리 실패
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& FailureString);	

private:
	TSubclassOf<UMenuWidgetBase> MainMenuWidgetClass;
	UMainMenu* MainMenuWidget;

	TSubclassOf<UMenuWidgetBase> PauseMenuWidgetClass;
	UMenuWidgetBase* PauseMenuWidget;

	FOnlineSessionSettings OnlineSessionSettings;
};