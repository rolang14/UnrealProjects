// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/RBInteractBase.h"
#include "Interact/RBCharacterInteractComponent.h"
#include "Interface/RBInteractInterface.h"

#include "Controller/RBPlayerController.h"
#include "Character/RBCharacterBase.h"

#include "UI/RBOnFieldHUDWidget.h"
#include "UI/RBDialogueBoxWidget.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


#pragma region Default Interact Transactions

URBInteractBase::URBInteractBase()
{
	bIsInteractable = false;
}

void URBInteractBase::ShowInteractToActor(const AActor* InActor)
{
	if (DefaultInteract == nullptr)
	{
		return;
	}

	// 디폴트 상호작용을 액터에게 띄운다. 
	if (IRBInteractInterface* InteractInterface = Cast<IRBInteractInterface>(InActor->GetComponentByClass(URBCharacterInteractComponent::StaticClass())))
	{
		InteractInterface->AddToInteract(DefaultInteract, GetOwner());
	}
}

void URBInteractBase::RemoveInteractFromActor(const AActor* InActor)
{
	if (DefaultInteract == nullptr)
	{
		return;
	}

	// 디폴트 상호작용을 액터에게서 지운다. 
	if (IRBInteractInterface* InteractInterface = Cast<IRBInteractInterface>(InActor->GetComponentByClass(URBCharacterInteractComponent::StaticClass())))
	{
		InteractInterface->DeleteFromInteract(DefaultInteract, GetOwner());
	}
}

#pragma endregion

void URBInteractBase::BeginDialogue(APlayerController* InInteractingPlayer)
{
	if (!ProcessPreDialogue(InInteractingPlayer))
	{
		return;
	}
	// 다이얼로그를 Show 하고 데이터로부터 텍스트를 불러와 진행
	OpenDialogue();
}

// 다이얼로그를 보여주기 위한 준비 단계
void URBInteractBase::OpenDialogue()
{
	// 다이얼로그 인덱스와 선택지 인덱스를 0으로 최초 설정
	CurrentDialogueIndex = 0;
	CurrentSelectNum = 0;

	// 플레이어의 인풋 모드를 변경
	//FInputModeGameAndUI InputMode;
	//InputMode.SetWidgetToFocus(DialogueBox->GetCachedWidget());
	//InteractingPlayer->SetInputMode(InputMode);

	InteractingPlayer->SetCurrentPlayerMode(EPlayerMode::FIELD_INTERACTING);

	// 다이얼로그 박스를 Visible 로 변경
	DialogueBox->SetVisibility(ESlateVisibility::Visible);
	//DialogueBox->SetKeyboardFocus();

	ReadyForNextSelect();
	ProcessNextDialogue();
}

// 선택지에 따른 분기 변경
void URBInteractBase::ReadyForNextSelect()
{
	if (InteractDialogueData[CurrentSelectNum].IsPending())
	{
		InteractDialogueData[CurrentSelectNum].LoadSynchronous();
	}

	CurrentDialogue.Empty();
	// 다이얼로그 데이터로부터 텍스트 파싱
	TArray<uint8*> InteractDataArr;
	InteractDialogueData[CurrentSelectNum]->GetRowMap().GenerateValueArray(InteractDataArr);
	Algo::Transform(InteractDataArr, CurrentDialogue,
		[](uint8* Value) {
			return *reinterpret_cast<FRBInteractDialogueData*>(Value);
		});
}

// 다음 다이얼로그 재생
void URBInteractBase::ProcessNextDialogue()
{
	UE_LOG(LogTemp, Display, TEXT("F Pressed in InteractBase"));

	if (CurrentDialogueIndex >= CurrentDialogue.Num())
	{
		CloseDialogue();
		return;
	}

	SetDialogueText(CurrentDialogueIndex);
	CurrentDialogueIndex++;
}

void URBInteractBase::SetDialogueText(const uint8 InDialogueIndex) const
{
	// 텍스트 설정
	DialogueBox->SetSpeakerText(CurrentDialogue[CurrentDialogueIndex].Speaker);
	DialogueBox->SetScriptText(CurrentDialogue[CurrentDialogueIndex].Script);
	// 보이스가 있다면, 보이스 재생
	if (CurrentDialogue[CurrentDialogueIndex].Voice && CurrentDialogue[0].Voice.IsPending())
	{
		USoundBase* VoiceSound = CurrentDialogue[CurrentDialogueIndex].Voice.LoadSynchronous();
		UGameplayStatics::PlaySoundAtLocation(this, VoiceSound, GetOwner()->GetActorLocation());
	}
}

#pragma region Close Dialogue

void URBInteractBase::CloseDialogue()
{
	ProcessPostDialogue();
	ClearInteractCamera();

	DialogueBox = nullptr;
	InteractingPlayer = nullptr;
}

void URBInteractBase::ProcessPostDialogue()
{
	// 다이얼로그 박스를 숨긴다.
	DialogueBox->CollapseDialogue();

	// 다이얼로그 박스 UI 세팅
	URBOnFieldHUDWidget* OnFieldWidget = Cast<URBOnFieldHUDWidget>(InteractingPlayer->GetOnFieldHUD());
	if (OnFieldWidget)
	{
		// Interact Widget 을 다시 보여준다.
		OnFieldWidget->SetInteractWidgetVisiblity(ESlateVisibility::Visible);
	}
}

void URBInteractBase::ClearInteractCamera()
{
	InteractingPlayer->SetCurrentPlayerMode(EPlayerMode::FIELD_NORMAL);
	// 원래 카메라로 돌아간다.
	InteractingPlayer->SetViewTargetWithBlend(InteractingPlayer->GetPawn(), CameraBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, CameraBlendExp);
	InteractingPlayer->SetIgnoreLookInput(false);
}

#pragma endregion

#pragma region Ready For Pre Dialogue

bool URBInteractBase::ProcessPreDialogue(APlayerController*& InInteractingPlayer)
{
	// 가능한 대화가 없으면 종료
	if (InteractDialogueData.Num() < 1)
	{
		return false;
	}

	// 플레이어 컨트롤러 세팅
	ARBPlayerController* RBPlayerController = Cast<ARBPlayerController>(InInteractingPlayer);
	if (RBPlayerController == nullptr)
	{
		return false;
	}
	InteractingPlayer = RBPlayerController;

	// 다이얼로그 박스 UI 세팅
	URBOnFieldHUDWidget* OnFieldWidget = Cast<URBOnFieldHUDWidget>(InteractingPlayer->GetOnFieldHUD());
	if (OnFieldWidget == nullptr)
	{
		return false;
	}
	// Interact Widget 을 일시적으로 가린다.
	OnFieldWidget->SetInteractWidgetVisiblity(ESlateVisibility::Hidden);

	DialogueBox = Cast<URBDialogueBoxWidget>(OnFieldWidget->WBP_DialogueBox);
	if (DialogueBox == nullptr)
	{
		return false;
	}

	// 플레이어와 NPC 위치 계산
	FVector PlayerPos = InteractingPlayer->GetPawn()->GetActorLocation();
	FVector NPCPos = GetOwner()->GetActorLocation();
	FVector CenterPos = (NPCPos + PlayerPos) / 2.0f;

	// 캐릭터와 NPC의 Rotation 을 마주보도록 변경
	SetCharacterRotation(InteractingPlayer->GetPawn(), PlayerPos, NPCPos, CenterPos);

	// 카메라를 InteractCamera로 이동
	SetInteractCamera(CenterPos);

	return true;
}

// Interact Camera 사전 세팅
void URBInteractBase::SetInteractCamera(const FVector& Center)
{
	if (InteractCamera == nullptr)
	{
		return;
	}

	InteractingPlayer->SetIgnoreLookInput(true);
	// 원래 카메라 Get
	OriginalCamera = InteractingPlayer->GetViewTarget();

	// 중간 지점으로부터 Dist 만큼 떨어져 있도록 카메라를 위치시킨다.
	FVector CameraForwardVector = InteractingPlayer->PlayerCameraManager->GetActorForwardVector();
	FVector NextPos = Center + -1 * (CameraForwardVector * 300);

	// 그 다음, 카메라가 CenterPos 를 바라보도록 회전시킨다.
	FRotator NextRot = UKismetMathLibrary::FindLookAtRotation(NextPos, Center);

	// 카메라 위치, 회전 벡터 변경
	InteractCamera->SetActorLocation(NextPos);
	InteractCamera->SetActorRotation(NextRot);

	// (향후) 만약 회전했지만 캐릭터가 NPC를 가릴 경우, NPC가 잘 보일 수 있도록 카메라를 왼쪽/오른쪽으로 30도 더 회전시킨다.
	// 1. Check Ray Trace Sweep from Camera to Player Character
	// 2. If HitResult, ReCalculate InteractCamera Position 30 deg from Center Point
	// 3. ReCalculate Camera Rotation from last position

	InteractingPlayer->SetViewTargetWithBlend(InteractCamera, CameraBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, CameraBlendExp);
}

// 캐릭터와 NPC 마주보게 하기
void URBInteractBase::SetCharacterRotation(APawn* PlayerPawn, const FVector& PlayerLoc, const FVector& NPCLoc, const FVector& CenterLoc)
{
	// 캐릭터와 NPC 캐릭터가 서로를 바라보도록 변경
	ARBCharacterBase* Character = Cast<ARBCharacterBase>(PlayerPawn);
	if (Character)
	{
		//DrawDebugAltCone(GetWorld(), PlayerPos, UKismetMathLibrary::FindLookAtRotation(PlayerPos, CenterPos), 100.f, 200.f, 100.f, FColor::Green, true);
		Character->TurnTo(UKismetMathLibrary::FindLookAtRotation(PlayerLoc, CenterLoc));
	}

	Character = nullptr;
	Character = Cast<ARBCharacterBase>(GetOwner());
	if (Character)
	{
		//DrawDebugAltCone(GetWorld(), NPCPos, UKismetMathLibrary::FindLookAtRotation(NPCPos, CenterPos), 100.f, 200.f, 100.f, FColor::Red, true);
		Character->TurnTo(UKismetMathLibrary::FindLookAtRotation(NPCLoc, CenterLoc));
	}
}

#pragma endregion