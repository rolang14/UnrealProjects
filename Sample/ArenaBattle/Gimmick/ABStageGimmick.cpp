// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Item/ABItemBox.h"
#include "Interface/ABGameInterface.h"
#include "Physics/ABCollision.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	// Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	// ABTRIGGER�� ���� �����ν� ���Ϳ��� ���������� �����ϰ� �ȴ�.
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);

	// Gate Section
	// �� ����Ʈ �̸� ����
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	if (GateMeshRef.Object)
	{
		for (FName GateSocket : GateSockets)
		{
			UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
			// �޽� ����
			Gate->SetStaticMesh(GateMeshRef.Object);
			// ���������� GateSocket ���Ͽ��ٰ� ����
			Gate->SetupAttachment(Stage, GateSocket);
			Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
			// ���� ���� (-90f�� ȸ��)
			Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
			Gates.Add({ GateSocket, Gate });

			// ����Ʈ�� Ʈ���� �ڽ�
			FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
			UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
			GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
			// ����Ʈ�� ������ ��ġ�� ������ �ȴ�.
			GateTrigger->SetupAttachment(Stage, GateSocket);
			GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
			GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
			GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
			// ������Ʈ �±� �߰�??
			// �̷��� �ϸ� ������Ʈ �±׿� GateSocket, �� ����Ʈ�� ��� ������ ����Ʈ������ �� �� �ְ� �Ǵ� ���̴�.
			GateTrigger->ComponentTags.Add(GateSocket);

			GateTriggers.Add(GateTrigger);
		}
	}

	// State Section
	CurrentState = EStageState::READY;

	StageChangedActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)));
	StageChangedActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)));
	StageChangedActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward)));
	StageChangedActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext)));

	// Fight Section
	// ���� �����̸� ����
	OpponentSpawnTime = 2.0f;
	// �� Ŭ������ AABCharacterNonPlayer �� ������.
	// �������̹Ƿ� ���̳����� �ƴ� StaticClass �� ����
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	// Reward Section
	// ������ �ڽ� Ŭ���� ����
	RewardBoxClass = AABItemBox::StaticClass();
	// �� ������ �ڽ��� ��ġ�� Gate Socket ��ġ�� �̿��Ͽ� ������.
	for (FName GateSocket : GateSockets)
	{
		// ���̶� ���� ��ġ�� �ȵǴϱ� / 2 �ؼ� �� �� �������� �����Ѵ�.
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		// ����Ʈ ���ϸ�� �����ϰ�, �׸��� ���� ��ġ�� �����Ѵ�.
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}

	// Stage Stat
	CurrentStageNum = 0;
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// ���� ���·� SetState
	SetState(CurrentState);
	UE_LOG(LogTemp, Log, TEXT("Set State"));
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �����ϸ� FIGHT �ܰ�� �̵�
	SetState(EStageState::FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �����ڿ��� ComponentTags�� �޾� �־����Ƿ� �翬�� �־�� ������, Ȥ�� �𸣴� �����ϰ� üũ. (�Ʒ� �������� �ʿ��ϹǷ�)
	check(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// ���� �̸� �� : +XGate ���� Left 2���� ���� +X, -X, +Y, -Y �� ������
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	// �׸��� �ش� ���� �̸��� �ִ��� �ٽ� ����üũ
	check(Stage->DoesSocketExist(SocketName));

	// Ȯ�εǸ�, �ش� ���� �̸��� ���� �ش� ����(����Ʈ)�� ��ġ�� �˾Ƴ�
	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	// ���� �ش� ��ġ�� �ٸ� ���������� ��ġ�Ǿ��ִٸ�
	TArray<FOverlapResult> OverlapResults;
	// ���� �� StageGimmick �� �����ϰ� �˻���.
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTriggers), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		// �������Ǵ³� �˻� ����� ���� �����ϰ�
		OverlapResults,
		// �ش� ���� ��ġ����
		NewLocation,
		// �ڱ� �ڽ��� ���ʹϾ𿡼�
		FQuat::Identity,
		// ��� ����ƽ ������Ʈ Ÿ�Կ� ����
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		// �� �� �ȿ� ���� ��� ������Ʈ�� �˻�
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	// �ߺ��� ������Ʈ�� ������
	if (!bResult)
	{
		FTransform NewTransform(NewLocation);
		// ���������� ��ȯ
		AABStageGimmick* NewStageGimmick = GetWorld()->SpawnActorDeferred<AABStageGimmick>(AABStageGimmick::StaticClass(), NewTransform);
		// �׸��� ���ο� ���������� ���� ����
		if (NewStageGimmick)
		{
			NewStageGimmick->SetStageNum(CurrentStageNum + 1);
			NewStageGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AABStageGimmick::OpenAllGates()
{
	UE_LOG(LogTemp, Log, TEXT("Gate Open"));

	for (auto Gate : Gates)
	{
		// ���� ���� ���·� �Ѵ�.
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		// ���� ���� ����(0, 0, 0)�� �Ѵ�.
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::SetState(const EStageState InNewState)
{
	// ���� ���¸� ���ο� ���·� �����ϰ�,
	CurrentState = InNewState;
	// ����� ���¿� ���ε��� �Լ��� �ִ� ���
	if (StageChangedActions.Contains(InNewState))
	{
		// �� �Լ��� ��������Ʈ���� ȣ���Ѵ�.
		StageChangedActions[CurrentState].StageChangedDelegate.ExecuteIfBound();
	}
}

// Stage ������ ��ٸ��� ����
void AABStageGimmick::SetReady()
{
	// Stage ���� Collision �� Ȱ��ȭ�ϰ�
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	// ������ ���鿡 ���ؼ��� Collision ���� �� �ݾ��ش�.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	OpenAllGates();
}

// Stage ���� �� Fight ������ ��ٸ��� ����
void AABStageGimmick::SetFight()
{
	// �ʿ���� ��� Ʈ���ſ� ���� ��Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates();

	// ���� ������ ���� ��ȯ
	// Ư�� ������(2��, ������ ����)�� ������ ���� �� StageGimmick ��ü�� OnOpponentSpawn�� �����ϵ��� ����
	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);
}

// Fight �� ���� ������ ��ٸ��� ����
void AABStageGimmick::SetChooseReward()
{
	// �ʿ���� ��� Ʈ���ſ� ���� ��Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates();

	SpawnBoxes();
}

// (���� ���� ��) ���� �������� ������ ��ٸ��� ����
void AABStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	// �̶��� ���� ���������� �����ؾ� �ϹǷ� ������ �� ���� �ش�.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}
	OpenAllGates();
}

void AABStageGimmick::OnOpponentDestroy(AActor* DestroyedActor)
{
	IABGameInterface* ABGameMode = Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());
	if (ABGameMode)
	{
		ABGameMode->OnPlayerScoreChanged(CurrentStageNum);
		if (ABGameMode->IsGameCleared())
		{
			return;
		}
	}
	// �� ���Ͱ� ������ REWARD ���·� ����
	SetState(EStageState::REWARD);
}

void AABStageGimmick::OnOpponentSpawn()
{
	// ������ ��ġ�� ���ϰ�
	/*const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;*/

	// ���͸� �����Ѵ�. �ٸ� TSubclassOf�� OpponentClass �� �����߱� ������, �ݵ�� NonPlayer ���͸� ��ȯ�� �ȴ�.
	// AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	// 
	// SpawnActor ��ſ� CurrentHp�� ����� �����ϱ� ���ؼ� BeginPlay�� ���� ������ �� �ִ�
	// SpawnActorDeferred �� ����Ѵ�. �̷��� �����ν� CurrentLevel ������ ��� CurrentHp�� ������ �� �ְ� �ȴ�.
	// ���ø� Ŭ������ NonPlayer�� ������ �� �ְ� �ǹǷ� ĳ������ �� �ʿ䵵 ��������.
	// ����, ���� ��ġ�� �����ϱ� ���ؼ��� �Ʒ��� ���� Transform�� ������ �־�� �Ѵ�.
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f);
	AABCharacterNonPlayer* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(OpponentClass, SpawnTransform);
	// ��� ĳ������ �翬�� �Ǿ߰�����, Ȥ�� �𸣴ϱ� �ѹ� �� Ȯ�����شٰ� ��.
	if (ABOpponentCharacter)
	{
		// ������ OnDestroyed ���ٰ� Gimmick�� OnOpponentDestroy �Լ��� ���ε�
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroy);
		// �� NPC ������ ����
		ABOpponentCharacter->SetLevel(CurrentStageNum);
		// ������ �������� �˸� (Deferred)
		ABOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// RewardBoxes �� ������� �������� ������ �߱� ������
	// �̰� ȣ��Ǵ� �������� Box�� �����ϴ��� �ƴ����� Ȯ���� �� ����.
	// ��������� �� ���Ͱ� �Ҹ����� �ʴ� �̻� Box�� �ݵ�� �Ҹ����� ������,
	// �������� ��Ÿ�ӿ��� Box�� �Ҹ��ϸ� �޸𸮿��� �����ǹǷ� �׻� �ִ��� �������� Ȯ���� �־�� �Ѵ�.
	for (const auto& RewardBox : RewardBoxes)
	{
		// ������ ��ü�� �����ϴ��� Ȯ��
		if (RewardBox.IsValid())
		{
			// �������� Get���� �������� �ȴ�.
			AABItemBox* ValidItemBox = RewardBox.Get();
			// ���� Overlap �� ������Ʈ�� ������ ���͸� �����´� (�ڽ�)
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			// �״ϱ� �̰Ŵ� ���� �÷��̾ RewardBoxes �߿��� �ϳ��� ������ �� �����Ƿ�
			// ������ �ȵ� �ڽ����� �� �Ҹ��Ų��.
			if (ValidItemBox != OverlappedBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}

void AABStageGimmick::SpawnBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FTransform WorldSpawnTransform(GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f));
		AABItemBox* ItemBox = GetWorld()->SpawnActorDeferred<AABItemBox>(RewardBoxClass, WorldSpawnTransform);
		if (ItemBox)
		{
			ItemBox->Tags.Add(RewardBoxLocation.Key);
			// ������ �ڽ��� Ʈ���Ÿ� �����ͼ� �� Ʈ������ OnComponentBeginOverlap�� �ش� �Լ� ���ε�
			ItemBox->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(ItemBox);
		}
	}

	// ��ü �ʱ�ȭ ���Ŀ� FinishSpawning �ϴ°����� �����Ѵ�.
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
	}
}

