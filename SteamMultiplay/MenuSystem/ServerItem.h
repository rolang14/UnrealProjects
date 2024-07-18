// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerItem.generated.h"


class UTextBlock;
class UButton;
class UMainMenu;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UServerItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(UMainMenu* InParent, const uint32 InIndex);
	void SetServerNameText(const FText InServerName);
	void SetHostNameText(const FText InHostName);
	void SetPlayerCountText(const FText InPlayerCount);

	// 현재 아이템을 선택된 상태로 바꾼다.
	FORCEINLINE void SetSelected(const bool bIsSelected) { Selected = bIsSelected; OnSelectedChanged(); }
	// 현재 아이템의 인덱스를 가져온다. (For Join Room List)
	FORCEINLINE const uint32 GetIndex() const { return Index; }

	// BP 구현
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectedChanged();
	
private:
	UFUNCTION()
	void OnItemClicked();
	
protected:
	// 서버 아이템 버튼 래퍼
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	UButton* Button_ServerItem;

private:
	// 서버 정보 : 서버 이름
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextBlock_ServerName;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock_ServerHostName;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock_ServerPlayerCount;

	UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess="true"))
	bool Selected = false;

	UMainMenu* Parent;
	uint32 Index;
};
