// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/MenuWidgetBase.h"
#include "MainMenu.generated.h"


class USystemButton;
class UServerScrollBox;
class UWidgetSwitcher;
class UButton;
class UEditableText;


USTRUCT()
struct FServerData
{
	GENERATED_BODY()

public:
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString ServerName;
	FString HostUserName;
};


/**
 *
 */
UCLASS()
class PUZZLEPLATFORM_API UMainMenu : public UMenuWidgetBase
{
	GENERATED_BODY()

public:
	// 위젯 생성자 : 놀랍게도 이거 CreateWidget 이 아니라 AddToViewport 해야 실행이 된다. 이걸 해야 UObject로 관리가 되는 것 같다.
	//virtual void NativeConstruct() override;

	// 위젯 소멸자
	virtual void NativeDestruct() override;

	// 기본적인 버튼 이벤트 동적 바인딩은 여기서 수행해야 한다.
	virtual bool Initialize() override;

	// Deprecated
	//void AddServerItem(const FString ServerName);

	// 배열로 관리하기
	void SetServerList(const TArray<FServerData> ServerDatas);

	// TOptional 인덱스를 설정
	void SetSelectedIndex(const uint32 Index);

	// UI에 입력한 룸 이름 반환
	const FText GetRoomName() const;

	// 로딩 화면 보이기 (원형)
	void ShowCircularLoadingScreen();
	// 로딩 화면 없애기 (원형)
	void HideCircularLoadingScreen();

private:
	// Create Room 메뉴에 진입
	UFUNCTION()
	void OpenCreateMenu();

	// Join Room 메뉴에 진입
	UFUNCTION()
	void OpenJoinMenu();

	// 호스팅 시작
	UFUNCTION()
	void CreateRoom();

	// 실제 호스트 서버에 Join
	UFUNCTION()
	void JoinRoom();

	UFUNCTION()
	void QuitGame();

	// 메인 메뉴로 돌아가기
	UFUNCTION()
	void GoBack();

	// 세선 검색 요청 함수
	UFUNCTION()
	void SessionSearchRequest();

	void ClearServerItem();

	// Child 들에게 인덱스를 반영하여 Selected 상태를 설정하기 위함
	void UpdateChildren();

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ServerItemWidgetClass;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CircularLoadingWidgetClass;

	UUserWidget* CircularLoadingWidget;

	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	// 예를 들어 위젯 그룹처럼 특정 유형의 위젯이 아니라 어떤 위젯이든 상관 없는 경우에는 최상위 위젯 클래스인 UWidget 을 쓰면 좋다.
	UPROPERTY(Meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(Meta = (BindWidget))
	UWidget* CreateMenu;

	// Main Menu Widget
	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_CreateMenu;

	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_JoinMenu;

	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_QuitGame;

	// Join Menu Widget
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* ScrollBox_Server;

	UPROPERTY(Meta = (BindWidget))
	UEditableText* EditableText_IpAddr;

	UPROPERTY(Meta = (BindWidget))
	UButton* Button_BackFromJoin;

	UPROPERTY(Meta = (BindWidget))
	UButton* Button_Refresh;

	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_JoinRoom;

	// Create Room Widget
	UPROPERTY(Meta = (BindWidget))
	UButton* Button_BackFromCreate;

	UPROPERTY(Meta = (BindWidget))
	USystemButton* WBP_SystemButton_CreateRoom;

	UPROPERTY(Meta = (BindWidget))
	UEditableText* EditableText_RoomName;

private:
	// 선택할 세션 인덱스
	TOptional<uint32> SelectedIndex;
};
