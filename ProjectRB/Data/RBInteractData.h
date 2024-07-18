// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RBInteractData.generated.h"

// 상호작용 타입 정의
UENUM(BlueprintType)
enum class EInteractType : uint8
{
	Default = 0,
	// 일반적인 대화
	Conversation,
	// 퀘스트
	Quest,
	// 조사하기 (루팅 등)
	Investigate,
	// 행동이 존재하는 액션 타입
	Action,
	Other
};

/**
 * 상호작용 타입과 UI에 표시될 메세지를 정의하는 데이터 에셋
 */
UCLASS()
class PROJECTRB_API URBInteractData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// 상호작용 타입
	UPROPERTY(EditAnywhere, Category = "Type")
	EInteractType InteractType;

	// UI 표시 메세지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	FText DisplayMessage;
};
