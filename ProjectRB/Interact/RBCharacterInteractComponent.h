// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interface/RBInteractInterface.h"
#include "Interface/RBDialogueInterface.h"

#include "RBCharacterInteractComponent.generated.h"


class URBInteractData;
// 상호작용 가능한 최대 행동 수
const int8 MaxInteractSize = 4;

// Interact Data <-> 위젯 통신을 처리할 구조체
USTRUCT(BlueprintType)
struct FRBGamePlayInteractData
{
	GENERATED_BODY()

public:
	FRBGamePlayInteractData() : InteractData(nullptr), Sender(nullptr) {}
	FRBGamePlayInteractData(const URBInteractData* InInteractData, AActor* InSender) : InteractData(InInteractData), Sender(InSender) {}

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	const URBInteractData* InteractData;

	UPROPERTY(BlueprintReadOnly, Category = "Sender")
	AActor* Sender;
};

// 상호작용 리스트가 변경되면 호출할 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractChangedDelegate, const TArray<const URBInteractData*>&/*Interact Data List*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedInteractChangedDelegate, const int32/*SelectedIndex*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRB_API URBCharacterInteractComponent : public UActorComponent, public IRBInteractInterface, public IRBDialogueInterface
{
	GENERATED_BODY()

public:	
	URBCharacterInteractComponent();

public:
	// 상호작용 목록 변화 시에 호출할 델리게이트
	FOnInteractChangedDelegate OnInteractChanged;
	FOnSelectedInteractChangedDelegate OnSelectedInteractChanged;

	// 상호작용 목록에 추가/제거
	virtual void AddToInteract(const URBInteractData* InteractData, AActor* Sender) override;
	virtual void DeleteFromInteract(const URBInteractData* InteractData, AActor* Sender) override;
	// 어떤 타입이든, 상호작용을 수행
	virtual void RunInteract(APlayerController* InteractingPlayer) override;

	FORCEINLINE const TArray<FRBGamePlayInteractData>& GetInteractList() const { return InteractList; }
	FORCEINLINE void RemoveItemFromInteractList(const int32 RemoveIndex) { InteractList.RemoveAt(RemoveIndex); }
	FORCEINLINE void AddItemToInteractList(const FRBGamePlayInteractData InData) { InteractList.Add(InData); }

	void ChangeInteractIndex(const int32 WheelValue);

// Dialogue Interact Section
public:
	virtual void ProcessDialogue() override;

private:
	// Interact Data 전송용 함수
	void SendInteractData() const;

	UPROPERTY(Transient)
	TArray<FRBGamePlayInteractData> InteractList;

	UPROPERTY(Transient)
	int32 SelectedInteractIndex;
};
