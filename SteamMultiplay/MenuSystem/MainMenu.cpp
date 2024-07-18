// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"
#include "MenuSystem/SystemButton.h"
#include "MenuSystem/ServerItem.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"


// 소멸될 때 UI 및 플레이어 입력 후처리
void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	ClearWidget();
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	if (!WBP_SystemButton_CreateMenu || !WBP_SystemButton_JoinMenu || !WBP_SystemButton_QuitGame ||
		!Button_BackFromJoin || !Button_Refresh || !WBP_SystemButton_JoinRoom ||
		!Button_BackFromCreate || !WBP_SystemButton_CreateRoom) return false;

	if (UButton* CreateMenuBtn = WBP_SystemButton_CreateMenu->ButtonHitArea) CreateMenuBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenCreateMenu);
	if (UButton* JoinMenuBtn = WBP_SystemButton_JoinMenu->ButtonHitArea) JoinMenuBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (UButton* QuitGame = WBP_SystemButton_QuitGame->ButtonHitArea) QuitGame->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	Button_BackFromJoin->OnClicked.AddDynamic(this, &UMainMenu::GoBack);
	Button_Refresh->OnClicked.AddDynamic(this, &UMainMenu::SessionSearchRequest);
	if (UButton* JoinBtn = WBP_SystemButton_JoinRoom->ButtonHitArea) JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinRoom);

	Button_BackFromCreate->OnClicked.AddDynamic(this, &UMainMenu::GoBack);
	if (UButton* CreateRoomBtn = WBP_SystemButton_CreateRoom->ButtonHitArea) CreateRoomBtn->OnClicked.AddDynamic(this, &UMainMenu::CreateRoom);

	return false;
}

// 서버 아이템을 스크롤 박스에 추가한다.
// Deprecated now
//void UMainMenu::AddServerItem(const FString ServerName)
//{
//	if (ServerItemWidgetClass == nullptr) return;
//	if (ScrollBox_Server == nullptr) return;
//
//	UServerItem* ServerItem = CreateWidget<UServerItem>(this, ServerItemWidgetClass);
//	if (ServerItem == nullptr) return;
//
//	ServerItem->SetServerItemName(FText::FromString(ServerName));
//	ScrollBox_Server->AddChild(ServerItem);
//}

#pragma region Search Callback : Add Server Item

// 서버 아이템 모두 제거 (제거 후 새로고침 된 데이터로 추가)
void UMainMenu::ClearServerItem()
{
	if (ScrollBox_Server == nullptr) return;
	ScrollBox_Server->ClearChildren();
}

// 배열로 관리하여 추가하기.
void UMainMenu::SetServerList(const TArray<FServerData> ServerDatas)
{
	ClearServerItem();

	uint32 Index = 0;
	for (const FServerData& Server : ServerDatas)
	{
		UServerItem* ServerItem = CreateWidget<UServerItem>(this, ServerItemWidgetClass);
		if (ServerItem == nullptr) return;

		ServerItem->Setup(this, Index++);
		ServerItem->SetServerNameText(FText::FromString(Server.ServerName));
		ServerItem->SetHostNameText(FText::FromString(Server.HostUserName));
		ServerItem->SetPlayerCountText(FText::FromString(FString::Printf(TEXT("%d/%d"), Server.CurrentPlayers, Server.MaxPlayers)));
		ScrollBox_Server->AddChild(ServerItem);
	}
}

// 선택된 서버 갱신 (For Join and UI)
void UMainMenu::SetSelectedIndex(const uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

const FText UMainMenu::GetRoomName() const
{
	return EditableText_RoomName->GetText();
}

// 선택된 서버를 기준으로 UI 변경 (For Selected-like Event)
void UMainMenu::UpdateChildren()
{
	for (auto Child : ScrollBox_Server->GetAllChildren())
	{
		UServerItem* Item = Cast<UServerItem>(Child);
		if (Item == nullptr) continue;

		Item->SetSelected(SelectedIndex.IsSet() && Item->GetIndex() == SelectedIndex.GetValue());
	}
}

#pragma endregion

#pragma region Main Menu

void UMainMenu::OpenCreateMenu()
{
	if (WidgetSwitcher == nullptr || CreateMenu == nullptr) return;
	WidgetSwitcher->SetActiveWidget(CreateMenu);
}

void UMainMenu::OpenJoinMenu()
{
	if (WidgetSwitcher == nullptr || JoinMenu == nullptr) return;
	WidgetSwitcher->SetActiveWidget(JoinMenu);

	// Join 메뉴에 진입하면서 자동으로 한번 온라인 세션을 불러온다
	SessionSearchRequest();
}

void UMainMenu::QuitGame()
{
	if (MenuInterface)
	{
		MenuInterface->QuitGame();
	}
}

#pragma endregion

#pragma region Join Room Menu

// Interface 를 통해 세션 서칭을 요청
void UMainMenu::SessionSearchRequest()
{
	if (MenuInterface)
	{
		MenuInterface->SearchSession();
	}
}

void UMainMenu::JoinRoom()
{
	// TOptional 값 확인. nullptr 일 수 있으므로 반드시 IsSet 으로 확인한 뒤 사용
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		MenuInterface->JoinSession(SelectedIndex.GetValue());
		UE_LOG(LogTemp, Display, TEXT("Selected Room Index : %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("There is no Selected Room"));
	}

	// 일시적으로 중단
	//if (EditableText_IpAddr == nullptr) return;
	//const FString& IpAddr = EditableText_IpAddr->GetText().ToString();
	//if (MenuInterface)
	//{
	//	MenuInterface->JoinSession(IpAddr);
	//}
}

#pragma endregion

#pragma region Create Room Menu

void UMainMenu::CreateRoom()
{
	if (EditableText_RoomName->GetText().IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("You should enter Name of the Room."));
		return;
	}

	if (MenuInterface)
	{
		MenuInterface->StartHosting();
	}
}

#pragma endregion

#pragma region Common

void UMainMenu::GoBack()
{
	if (WidgetSwitcher == nullptr || MainMenu == nullptr) return;
	WidgetSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ShowCircularLoadingScreen()
{
	if (CircularLoadingWidgetClass == nullptr || CircularLoadingWidget != nullptr) return;
	CircularLoadingWidget = CreateWidget(this, CircularLoadingWidgetClass);
	CircularLoadingWidget->AddToViewport();
}

void UMainMenu::HideCircularLoadingScreen()
{
	if (CircularLoadingWidget == nullptr) return;
	CircularLoadingWidget->RemoveFromViewport();
	CircularLoadingWidget = nullptr;
}

#pragma endregion