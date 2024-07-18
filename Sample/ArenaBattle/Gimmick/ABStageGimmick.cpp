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
	// ABTRIGGER로 정해 줌으로써 액터와의 오버랩만을 감지하게 된다.
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);

	// Gate Section
	// 각 게이트 이름 지정
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	if (GateMeshRef.Object)
	{
		for (FName GateSocket : GateSockets)
		{
			UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
			// 메시 세팅
			Gate->SetStaticMesh(GateMeshRef.Object);
			// 스테이지의 GateSocket 소켓에다가 붙임
			Gate->SetupAttachment(Stage, GateSocket);
			Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
			// 열린 상태 (-90f로 회전)
			Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
			Gates.Add({ GateSocket, Gate });

			// 게이트용 트리거 박스
			FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
			UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
			GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
			// 게이트와 동일한 위치에 놓으면 된다.
			GateTrigger->SetupAttachment(Stage, GateSocket);
			GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
			GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
			GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
			// 컴포넌트 태그 추가??
			// 이렇게 하면 컴포넌트 태그에 GateSocket, 즉 게이트가 어느 방향의 게이트인지를 알 수 있게 되는 것이다.
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
	// 스폰 딜레이를 지정
	OpponentSpawnTime = 2.0f;
	// 적 클래스를 AABCharacterNonPlayer 로 지정함.
	// 생성자이므로 다이나믹이 아닌 StaticClass 로 생성
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	// Reward Section
	// 아이템 박스 클래스 설정
	RewardBoxClass = AABItemBox::StaticClass();
	// 각 아이템 박스의 위치는 Gate Socket 위치를 이용하여 설정함.
	for (FName GateSocket : GateSockets)
	{
		// 문이랑 같은 위치면 안되니까 / 2 해서 좀 더 안쪽으로 설정한다.
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		// 게이트 소켓명과 동일하게, 그리고 계산된 위치로 지정한다.
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}

	// Stage Stat
	CurrentStageNum = 0;
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// 현재 상태로 SetState
	SetState(CurrentState);
	UE_LOG(LogTemp, Log, TEXT("Set State"));
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 진입하면 FIGHT 단계로 이동
	SetState(EStageState::FIGHT);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 생성자에서 ComponentTags를 달아 주었으므로 당연히 있어야 하지만, 혹시 모르니 강력하게 체크. (아래 로직에서 필요하므로)
	check(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// 소켓 이름 예 : +XGate 에서 Left 2개를 통해 +X, -X, +Y, -Y 를 가져옴
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	// 그리고 해당 소켓 이름이 있는지 다시 더블체크
	check(Stage->DoesSocketExist(SocketName));

	// 확인되면, 해당 소켓 이름을 통해 해당 소켓(게이트)의 위치를 알아냄
	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	// 만약 해당 위치에 다른 스테이지가 설치되어있다면
	TArray<FOverlapResult> OverlapResults;
	// 지금 이 StageGimmick 을 제외하고 검사함.
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTriggers), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		// 오버랩되는놈 검사 결과를 여기 저장하고
		OverlapResults,
		// 해당 소켓 위치에서
		NewLocation,
		// 자기 자신의 쿼터니언에서
		FQuat::Identity,
		// 모든 스태틱 오브젝트 타입에 대해
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		// 이 원 안에 들어가는 모든 오브젝트를 검사
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	// 중복된 오브젝트가 없으면
	if (!bResult)
	{
		FTransform NewTransform(NewLocation);
		// 스테이지를 소환
		AABStageGimmick* NewStageGimmick = GetWorld()->SpawnActorDeferred<AABStageGimmick>(AABStageGimmick::StaticClass(), NewTransform);
		// 그리고 새로운 스테이지의 레벨 설정
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
		// 문을 열린 상태로 한다.
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		// 문을 닫힌 상태(0, 0, 0)로 한다.
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::SetState(const EStageState InNewState)
{
	// 현재 상태를 새로운 상태로 변경하고,
	CurrentState = InNewState;
	// 변경된 상태에 바인딩된 함수가 있는 경우
	if (StageChangedActions.Contains(InNewState))
	{
		// 그 함수를 델리게이트에게 호출한다.
		StageChangedActions[CurrentState].StageChangedDelegate.ExecuteIfBound();
	}
}

// Stage 진입을 기다리는 상태
void AABStageGimmick::SetReady()
{
	// Stage 진입 Collision 을 활성화하고
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	// 나머지 문들에 대해서는 Collision 없앤 뒤 닫아준다.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	OpenAllGates();
}

// Stage 진입 후 Fight 세팅을 기다리는 상태
void AABStageGimmick::SetFight()
{
	// 필요없는 모든 트리거와 문을 비활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates();

	// 문이 닫히면 적을 소환
	// 특정 딜레이(2초, 생성자 지정)가 지나면 지금 이 StageGimmick 객체의 OnOpponentSpawn을 실행하도록 지정
	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);
}

// Fight 후 보상 세팅을 기다리는 상태
void AABStageGimmick::SetChooseReward()
{
	// 필요없는 모든 트리거와 문을 비활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates();

	SpawnBoxes();
}

// (보상 선택 후) 다음 스테이지 진행을 기다리는 상태
void AABStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	// 이때는 다음 스테이지로 진입해야 하므로 문들을 다 열어 준다.
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
	// 적 액터가 죽으면 REWARD 상태로 변경
	SetState(EStageState::REWARD);
}

void AABStageGimmick::OnOpponentSpawn()
{
	// 스폰할 위치를 정하고
	/*const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;*/

	// 액터를 스폰한다. 다만 TSubclassOf로 OpponentClass 를 한정했기 때문에, 반드시 NonPlayer 액터만 소환이 된다.
	// AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	// 
	// SpawnActor 대신에 CurrentHp를 제대로 세팅하기 위해서 BeginPlay를 지연 실행할 수 있는
	// SpawnActorDeferred 를 사용한다. 이렇게 함으로써 CurrentLevel 설정한 대로 CurrentHp를 변경할 수 있게 된다.
	// 탬플릿 클래스로 NonPlayer를 지정할 수 있게 되므로 캐스팅을 할 필요도 없어진다.
	// 또한, 이제 위치를 지정하기 위해서는 아래와 같이 Transform을 정의해 주어야 한다.
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f);
	AABCharacterNonPlayer* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(OpponentClass, SpawnTransform);
	// 사실 캐스팅은 당연히 되야겠지만, 혹시 모르니까 한번 더 확인해준다고 함.
	if (ABOpponentCharacter)
	{
		// 액터의 OnDestroyed 에다가 Gimmick의 OnOpponentDestroy 함수를 바인딩
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroy);
		// 적 NPC 레벨을 세팅
		ABOpponentCharacter->SetLevel(CurrentStageNum);
		// 스폰이 끝났음을 알림 (Deferred)
		ABOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// RewardBoxes 는 헤더에서 약참조로 선언을 했기 때문에
	// 이게 호출되는 시점에서 Box가 존재하는지 아닌지를 확신할 수 없다.
	// 강참조라면 본 액터가 소멸하지 않는 이상 Box가 반드시 소멸하지 않지만,
	// 약참조는 런타임에서 Box가 소멸하면 메모리에서 삭제되므로 항상 있는지 없는지를 확인해 주어야 한다.
	for (const auto& RewardBox : RewardBoxes)
	{
		// 약참조 객체가 존재하는지 확인
		if (RewardBox.IsValid())
		{
			// 약참조는 Get으로 가져오면 된다.
			AABItemBox* ValidItemBox = RewardBox.Get();
			// 현재 Overlap 된 컴포넌트를 소유한 액터를 가져온다 (박스)
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			// 그니까 이거는 이제 플레이어가 RewardBoxes 중에서 하나만 선택할 수 있으므로
			// 선택이 안된 박스들은 다 소멸시킨다.
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
			// 아이템 박스의 트리거를 가져와서 그 트리거의 OnComponentBeginOverlap에 해당 함수 바인딩
			ItemBox->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(ItemBox);
		}
	}

	// 전체 초기화 이후에 FinishSpawning 하는것으로 변경한다.
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
	}
}

