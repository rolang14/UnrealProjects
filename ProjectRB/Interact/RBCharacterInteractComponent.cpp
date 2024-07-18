// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/RBCharacterInteractComponent.h"
#include "Interact/RBInteractBase.h"
#include "Data/RBInteractData.h"


// Sets default values for this component's properties
URBCharacterInteractComponent::URBCharacterInteractComponent()
{
	SelectedInteractIndex = 0;
}

#pragma region Control Interact Data

// 들어온 상호작용 데이터를 상호작용 가능한 목록에 추가한다.
void URBCharacterInteractComponent::AddToInteract(const URBInteractData* InteractData, AActor* Sender)
{
	if (InteractData == nullptr)
	{
		return;
	}

	// 최대 개수 이상은 넣을 수 없음
	if (GetInteractList().Num() >= MaxInteractSize)
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s is added to Interact List."), *InteractData->DisplayMessage.ToString());

	// 상호작용 목록에 추가
	AddItemToInteractList(FRBGamePlayInteractData(InteractData, Sender));

	// Delegate BroadCast
	SendInteractData();
	ChangeInteractIndex(0);
}

// 해당 상호작용 데이터를 목록에서 삭제한다.
void URBCharacterInteractComponent::DeleteFromInteract(const URBInteractData* InteractData, AActor* Sender)
{
	if (InteractData == nullptr)
	{
		return;
	}

	for (int i = 0; i < GetInteractList().Num(); i++)
	{
		if (GetInteractList()[i].Sender == Sender && GetInteractList()[i].InteractData == InteractData)
		{
			// 목록에서 삭제 후 UI에 알림
			UE_LOG(LogTemp, Display, TEXT("%s is Deleted from Interact List."), *GetInteractList()[i].InteractData->DisplayMessage.ToString());
			RemoveItemFromInteractList(i);
			SendInteractData();

			// Selected Interact Index 가 사라진 위젯을 가리키고 있다면 이전 위젯을 가리키도록 한다.
			if (SelectedInteractIndex >= GetInteractList().Num())
			{
				ChangeInteractIndex(-1);
			}
			// 아니라면 현재 인덱스를 유지
			else
			{
				ChangeInteractIndex(0);
			}
			break;
		}
	}
}

// 위젯에서는 Sender 정보가 필요 없으므로, 상호작용 데이터만 따로 리스트로 묶어서 전달하기 위함
void URBCharacterInteractComponent::SendInteractData() const
{
	TArray<const URBInteractData*> SendData;
	for (const FRBGamePlayInteractData& it : InteractList)
	{
		SendData.Add(it.InteractData);
		//UE_LOG(LogTemp, Display, TEXT("%s is in Interact List."), *it.InteractData->DisplayMessage.ToString());
	}
	OnInteractChanged.Broadcast(SendData);
}

void URBCharacterInteractComponent::ChangeInteractIndex(const int32 WheelValue)
{
	SelectedInteractIndex = FMath::Clamp(SelectedInteractIndex + WheelValue, 0, GetInteractList().Num() - 1);
	OnSelectedInteractChanged.Broadcast(SelectedInteractIndex);
}

#pragma endregion

#pragma region Run Interact

// 현재 선택된 상호작용 실행
void URBCharacterInteractComponent::RunInteract(APlayerController* InteractingPlayer)
{
	if (InteractList.Num() == 0)
	{
		return;
	}

	// 상호작용 타입에 따라서 매칭되는 상호작용 실행
	// 대화 타입 -> BeginDialogue
	// 아이템 루팅 타입 -> 뭐.. GetItem 이런식으로
	switch (InteractList[SelectedInteractIndex].InteractData->InteractType)
	{
	case EInteractType::Default:
		if (URBInteractBase* InteractRef = Cast<URBInteractBase>(InteractList[SelectedInteractIndex].Sender->GetComponentByClass(URBInteractBase::StaticClass())))
		{
			InteractRef->BeginDialogue(InteractingPlayer);
		}
		break;
	case EInteractType::Other:
		// Use Other Interact Interface
		break;
	}
}

#pragma endregion

#pragma region Dialogue Interact

void URBCharacterInteractComponent::ProcessDialogue()
{
	if (URBInteractBase* InteractRef = Cast<URBInteractBase>(InteractList[SelectedInteractIndex].Sender->GetComponentByClass(URBInteractBase::StaticClass())))
	{
		InteractRef->ProcessNextDialogue();
	}
}

#pragma endregion