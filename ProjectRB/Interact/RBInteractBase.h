// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RBInteractDialogueData.h"
#include "RBInteractBase.generated.h"


class URBInteractData;
class UDataTable;
class URBDialogueBoxWidget;
class ARBPlayerController;

/*
*
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRB_API URBInteractBase : public UActorComponent
{
	GENERATED_BODY()
	
public:
	URBInteractBase();

	// Overlap 시에 캐릭터에게 디폴트 상호작용을 보여 준다.
	void ShowInteractToActor(const AActor* InActor);
	void RemoveInteractFromActor(const AActor* InActor);

	// 다이얼로그 진입
	void BeginDialogue(APlayerController* InInteractingPlayer);

	// 다음 다이얼로그를 실행
	UFUNCTION(BlueprintCallable)
	void ProcessNextDialogue();

	// 상호작용 데이터 (표시용)
	UPROPERTY(EditInstanceOnly, Category = "Interact Data")
	TObjectPtr<URBInteractData> DefaultInteract;

	// 실제 다이얼로그 데이터
	UPROPERTY(EditInstanceOnly, Category = "Interact Dialogue Data")
	TArray<TSoftObjectPtr<UDataTable>> InteractDialogueData;

	UPROPERTY(EditInstanceOnly, Category = "Interact Camera Motion")
	float CameraBlendTime = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Interact Camera Motion")
	float CameraBlendExp = 1.0f;

// For Dialogue Func
private:
	// 상호작용을 비추는 카메라 설정
	void SetInteractCamera(const FVector& Center);
	// 플레이어와 NPC 캐릭터 방향 설정
	void SetCharacterRotation(APawn* PlayerPawn, const FVector& PlayerLoc, const FVector& NPCLoc, const FVector& CenterLoc);

	bool ProcessPreDialogue(APlayerController*& InInteractingPlayer);
	void ProcessPostDialogue();

	// 다이얼로그 시작
	void OpenDialogue();
	// 다이얼로그 종료
	void CloseDialogue();

	void ClearInteractCamera();

	// 선택지 분기 변경
	void ReadyForNextSelect();

	// 다이얼로그 텍스트 설정
	void SetDialogueText(const uint8 InDialogueIndex) const;

private:
	// 상호작용 테이크 카메라
	UPROPERTY(EditInstanceOnly, Category = "Interact Camera")
	TObjectPtr<AActor> InteractCamera;

	UPROPERTY()
	TObjectPtr<AActor> OriginalCamera;

	// 다이얼로그를 표시할 위젯
	UPROPERTY()
	TObjectPtr<URBDialogueBoxWidget> DialogueBox;

	// 현재 Interact 중인 Player
	UPROPERTY()
	TObjectPtr<ARBPlayerController> InteractingPlayer;

	// 다이얼로그 데이터 파싱용
	TArray<FRBInteractDialogueData> CurrentDialogue;

	// 다이얼로그 재생을 위한 인덱스
	uint8 CurrentDialogueIndex;
	// 선택지 분기를 위한 인덱스
	uint8 CurrentSelectNum;

	UPROPERTY(BlueprintReadOnly, Category = "Interact", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsInteractable : 1;
};
