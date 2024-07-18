// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PuzzlePlatformGameInstance.h"
#include "MenuSystem/MenuWidgetBase.h"
#include "MenuSystem/MainMenu.h"

#include "Blueprint/UserWidget.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemSteam.h"


// 임의로 정의해서 사용하면, 언리얼 엔진에서 사용하는 게임 세션의 이름을 반영하지 못하고, 일부 세선 정보가 이 때문에 누락되는 현상이 발생한다
// 때문에 언리얼에서 제공하는 Name_GameSession 을 대신 사용하자.
//const static FName SESSION_NAME = TEXT("Game");
const static FName SESSION_SETTING_KEY_ROOMNAME = TEXT("RoomName");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMainMenu> MenuWidgetClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MenuWidgetClass.Class == nullptr) return;

	MainMenuWidgetClass = MenuWidgetClass.Class;

	static ConstructorHelpers::FClassFinder<UMenuWidgetBase> PauseWidgetClass(TEXT("/Game/MenuSystem/WBP_PauseMenu"));
	if (PauseWidgetClass.Class == nullptr) return;

	PauseMenuWidgetClass = PauseWidgetClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));

	// 온라인 서브시스템 구현. OSS는 싱글톤의 형태로 존재한다.
	if (IOnlineSubsystem* OSS = IOnlineSubsystem::Get())
	{
		// 디폴트 서브시스템은 Engine.ini 에 작성한 설정을 따라가게 되어있다.
		// 스팀용이면 스팀을 쓰면 되고, 개발용이라면 일단 그냥 NULL을 넣어서 null 이라는 개발용 온라인 서브시스템을 사용하면 된다. 즉 이런 경우에 아래의 코드를 실행하면 NULL 이라는 이름이 찍힌다.
		UE_LOG(LogTemp, Warning, TEXT("Current Online Subsystem System : %s"), *OSS->GetSubsystemName().ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Online Subsystem System : %s"), *OSS->GetSubsystemName().ToString()));
		}
		// 매치메이킹을 위한 세션 인터페이스를 온라인 서브시스템으로부터 가져온다.
		SessionInterfacePtr = OSS->GetSessionInterface();
		if (SessionInterfacePtr.IsValid())
		{
			SessionInterfacePtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterfacePtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterfacePtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionComplete);
			SessionInterfacePtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
		}
	}

	//if (GEngine)
	//{
	//	// 5.x버전에선 해결되었지만, 호스트가 도중에 나갈 경우 나머지 클라이언트들이 처리되지 않아 멈추는 문제가 있었다.
	//	// 이를 위해 엔진에서의 델리게이트를 구독하여 네트워크 처리 실패를 처리하도록 한다.
	//	// 실제로 대부분의 네트워크 에러 처리가 세션 인터페이스가 아닌 엔진 수준에서 이루어진다.
	//	GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformGameInstance::OnNetworkFailure);
	//}
}

// 임시로 LoadMainMenu 에 세션 콜백을 두어서 세션을 생성해야만 게임에 진입하도록 만든다. 이를 위해 델리게이트 인자들을 맞춰 준다.
void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	if (MainMenuWidgetClass == nullptr) return;

	MainMenuWidget = CreateWidget<UMainMenu>(this, MainMenuWidgetClass);
	if (MainMenuWidget == nullptr) return;

	MainMenuWidget->SetMenuInterface(this);
	MainMenuWidget->SetUp();
}

void UPuzzlePlatformGameInstance::LoadPauseMenu()
{
	if (PauseMenuWidgetClass == nullptr) return;

	PauseMenuWidget = CreateWidget<UMenuWidgetBase>(this, PauseMenuWidgetClass);
	if (PauseMenuWidget == nullptr) return;

	PauseMenuWidget->SetMenuInterface(this);
	PauseMenuWidget->SetUp();
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	UE_LOG(LogTemp, Display, TEXT("Session %s was Successfully Destroyed."), *SessionName.ToString());
	CreateSession();
}

void UPuzzlePlatformGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& FailureString)
{
	LoadMainMenu();
}

void UPuzzlePlatformGameInstance::StartHosting()
{
	if (SessionInterfacePtr.IsValid())
	{
		if (auto ExistingSession = SessionInterfacePtr->GetNamedSession(NAME_GameSession))
		{
			UE_LOG(LogTemp, Display, TEXT("Session already Exists and trying to Destroy..."));
			SessionInterfacePtr->DestroySession(ExistingSession->SessionName);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterfacePtr.IsValid())
	{
		MainMenuWidget->ShowCircularLoadingScreen();

		// 이 세션을 LAN 세션으로 만든다.
		// Steam OSS 사용중에는 LAN 매치를 사용하면 안되것지? (상황에 따라 랜 매치를 지원하는 경우(스타크래프트) 뭐 할 수도 있겠지만 요즘은 랜 매치 지원 안한다)
		// 어쨌든 NULL OSS 일때만 LAN Match를 하도록 한다.
		OnlineSessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("NULL"));

		// 이 세션이 다른 사용자에게 보이도록 한다.
		OnlineSessionSettings.bShouldAdvertise = true;
		// Public 상태의 세션 이용자 (Advertise 등으로 입장한 이용자) 수를 2명으로 한다.
		OnlineSessionSettings.NumPublicConnections = 5;
		// 반대로 Private 는 Invite 혹은 Password 입장한 연결을 말한다.

		// Lobby 사용을 위해 Presence 를 활성화
		OnlineSessionSettings.bUsesPresence = true;
		OnlineSessionSettings.bUseLobbiesIfAvailable = true;
		OnlineSessionSettings.bAllowJoinViaPresence = true;

		// 세션 메타 데이터 설정
		// 정해진 키에 매칭되는 Value를 설정한다. 이를 세션을 찾을 때에도 가져와서 사용할 수 있다. 여기서는 방의 이름을 설정한다.
		OnlineSessionSettings.Set(SESSION_SETTING_KEY_ROOMNAME, FString(MainMenuWidget->GetRoomName().ToString()), EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

		// 세션을 만든다. 서브시스템을 어떤 것을 사용할지 모르기 때문에 기본적으로 델리게이트의 이벤트 형태로 작동하며
		// 또한 세션 연결, 생성 중 게임이 멈추지 않도록 하기 위해 비동기적으로 수행하려는 목적도 있다.
		SessionInterfacePtr->CreateSession(0, NAME_GameSession, OnlineSessionSettings);
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// or GetEngine()
	if (GEngine)
	{
		// Key 가 -1 이면 기존 메세지를 Overwrite 하지 않고 새로운 메세지를 추가한다.
		// 반대로, 0이면 기존 메세지를 덮어쓰고 이 메세지를 출력한다.
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Ready for Hosting . . ."));
	}

	if (!bWasSuccessful) return;

	MainMenuWidget->HideCircularLoadingScreen();

	UE_LOG(LogTemp, Display, TEXT("[Callback] Current Session Name : %s"), *SessionName.ToString());

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	// 리슨 서버로 호스팅
	World->ServerTravel(TEXT("/Game/PuzzlePlatform/Maps/Lobby?listen"));
}

// 퍼블릭 세선 찾기
void UPuzzlePlatformGameInstance::SearchSession()
{
	// TSharedPtr 로 만들기 위해 동적 할당을 해야 한다.
	// 동적 할당을 그냥 해버리면 굉장히 치명적일 수 있다. 때문에 MakeSharable 을 사용하여 동적할당한 레퍼런스를 참조할 수 있도록 설정한다.
	// 또한, TSharedRef 는 반드시 유효해야 하므로 초기화하여 사용하면 컴파일이 안될 수 있다. 컴파일 되더라도, 여전히 유효하지 않을 가능성이 있는 한 Assert 된다.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		MainMenuWidget->ShowCircularLoadingScreen();

		SessionSearch->MaxSearchResults = 100;
		SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("NULL"));
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		// 때문에 이와 같이 TSharedPtr 을 사용하여 초기화 한 뒤, Valid 한 경우에만 ToSharedRef 로 공유 레퍼런스를 가져올 수 있다.
		//SessionInterfacePtr->FindSessions(0, SessionSearch.ToSharedRef());
		SessionInterfacePtr->FindSessions(0, SessionSearch.ToSharedRef());

		UE_LOG(LogTemp, Display, TEXT("Trying to Search Session..."));
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("Find Session Completed."));
	if (!bWasSuccessful || !SessionSearch.IsValid() || !MainMenuWidget) return;

	// 세션 찾기 완료 후, 세션이 존재한다면 UI에 추가한다.
	//MainMenuWidget->ClearServerItem();

	//TArray<FOnlineSessionSearchResult>& SearchResult = SessionSearch->SearchResults;
	//if (SearchResult.Num() > 0)
	//{
	//	for (auto& Result : SearchResult)
	//	{
	//		MainMenuWidget->AddServerItem(Result.GetSessionIdStr());
	//		UE_LOG(LogTemp, Display, TEXT("Found Session's Id : %s"), *Result.GetSessionIdStr());
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Display, TEXT("But There's no Session available."));
	//}

	MainMenuWidget->HideCircularLoadingScreen();

	// 배열로 관리하도록 하기.
	TArray<FServerData> ServerDatas;
	TArray<FOnlineSessionSearchResult>& SearchResult = SessionSearch->SearchResults;
	if (SearchResult.Num() > 0)
	{
		for (auto& Result : SearchResult)
		{
			FServerData ServerData;
			ServerData.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = ServerData.MaxPlayers - Result.Session.NumOpenPublicConnections;
			ServerData.HostUserName = Result.Session.OwningUserName;
			FString RoomName;
			if (Result.Session.SessionSettings.Get(SESSION_SETTING_KEY_ROOMNAME, RoomName))
			{
				ServerData.ServerName = RoomName;
			}
			else
			{
				ServerData.ServerName = Result.GetSessionIdStr();
			}
			
			ServerDatas.Add(ServerData);
		}
	}

	MainMenuWidget->SetServerList(ServerDatas);
}

// 원하는 세션에 참가 (UI로부터 선택된 Index를 받아와서 처리한다)
void UPuzzlePlatformGameInstance::JoinSession(const uint32 Index)
{
	if (!SessionInterfacePtr.IsValid() || !SessionSearch.IsValid()) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Joining to Session No.%d ..."), Index));
	}

	// 특정 세션에 참가 시도
	if (unsigned(SessionSearch->SearchResults.Num()) > Index)
	{
		switch (SessionInterfacePtr->GetSessionState(NAME_GameSession))
		{
		case EOnlineSessionState::InProgress:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : InProgress"));
			break;
		case EOnlineSessionState::Creating:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : Creating"));
			break;
		case EOnlineSessionState::Destroying:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : Destroying"));
			break;
		case EOnlineSessionState::Ended:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : Ended"));
			break;
		case EOnlineSessionState::Ending:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : Ending"));
			break;
		case EOnlineSessionState::NoSession:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : NoSession"));
			break;
		case EOnlineSessionState::Pending:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : Pending"));
			break;
		case EOnlineSessionState::Starting:
			UE_LOG(LogTemp, Warning, TEXT("Selected Session State : Starting"));
			break;
		}

		SessionInterfacePtr->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
	}
}

// 원하는 세션에 참가 시도 결과
void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionResult)
{
	if (!SessionInterfacePtr.IsValid()) return;

	if (JoinSessionResult == EOnJoinSessionCompleteResult::Type::Success)
	{
		// 성공했다면, 결과로 연결할 세션의 연결 정보(IPAddr, Port)가 담긴다.
		FString ConnectInfo;
		if (!SessionInterfacePtr->GetResolvedConnectString(NAME_GameSession, ConnectInfo))
		{
			UE_LOG(LogTemp, Display, TEXT("Cannot Find Session ConnectInfo."));
			return;
		}

		UE_LOG(LogTemp, Display, TEXT("Successfully Joined Session. Results : %s"), *ConnectInfo);

		// 해당 연결 정보로 Client Travel 하여 접속.
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (PlayerController == nullptr) return;

		//SessionInterfacePtr->UpdateSession(NAME_GameSession, OnlineSessionSettings);
		PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
	}
	else if (JoinSessionResult == EOnJoinSessionCompleteResult::Type::AlreadyInSession)
	{
		UE_LOG(LogTemp, Display, TEXT("You are already in Session : %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to join Session : %s"), *SessionName.ToString());
	}
}

void UPuzzlePlatformGameInstance::StartSession()
{
	if (SessionInterfacePtr == nullptr) return;
	SessionInterfacePtr->StartSession(NAME_GameSession);
}

void UPuzzlePlatformGameInstance::LeaveGame()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Leave Game"));
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;

	// 이렇게 하면 서버 쪽에서는 자동으로 클라이언트의 연결을 해제하게 된다.
	PlayerController->ClientTravel(TEXT("/Game/PuzzlePlatform/Maps/MainMenu"), ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::QuitGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ConsoleCommand(TEXT("quit"));
}