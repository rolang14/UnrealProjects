// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Physics/ABCollision.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/ABCharacterItemInterface.h"
// 애셋 매니저 사용을 위한 헤더
#include "Engine/AssetManager.h"
#include "ABItemData.h"

// Sets default values
AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	// 기존에 만들어 놓았던 (혹은 새로 만들거나) 트리거용 Collision Profile 으로 설정함.
	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// 기존 메시 사용
	ConstructorHelpers::FObjectFinder<UStaticMesh>BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	// 메시 자체는 충돌이 없음.
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	// 이펙트 역시 기존 애셋 사용
	ConstructorHelpers::FObjectFinder<UParticleSystem>BoxOpenEffectRef(TEXT("/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (BoxOpenEffectRef.Object)
	{
		// 파티클 시스템은 SetTemplate 로 설정
		Effect->SetTemplate(BoxOpenEffectRef.Object);
		// 시작하자마자 발동하는게 아니라, 습득했을 때 발동해야 함.
		Effect->bAutoActivate = false;
	}
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 애셋 매니저는 싱글톤 인스턴스이므로 엔진이 초기화되면 반드시 존재한다.
	// 애셋 매니저를 불러와서
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	// PrimarytAssetIdList 를 받아온다.
	// 우리가 프로젝트 세팅에서 지정한 태그명 (ABItemData) 에 지정된
	// 경로 내에서 이러한 태그명을 가지고 있는 애셋들의 목록을 모두 나열해 준다.
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	ensure(0 < Assets.Num());

	// 랜덤으로 인덱스 선택
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	// 약참조를 걸어서 로딩을 직접 시켜주자.
	// 에셋 경로를 가져와서
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	// 살아있는 UObject가 아니라면
	if (AssetPtr.IsPending())
	{
		// 로딩해준다.
		AssetPtr.LoadSynchronous();
	}
	// 로딩된 애셋을 넘겨준다.
	Item = Cast<UABItemData>(AssetPtr.Get());
	ensure(Item);

	// Trigger의 OnComponentBeginOverlap은 Dynamic으로 선언이 된 델리게이트 이므로 우리가 앞서 연결 함수에 UFUNCTION()으로 지정해 준 것이다.
	// Spawn 타이밍 조절을 위해 PostInitializeComponent 로 변경
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
}

// 오버랩되면 발동하는 함수
void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// 아이템이 없다면 그냥 삭제
	if (Item == nullptr)
	{
		Destroy();
		return;
	}

	// 아이템을 획득하면 액터에게 Take Item 함수를 호출하도록 시킨다.
	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	// 이펙트를 발동하고
	Effect->Activate(true);
	// Mesh를 숨긴 뒤
	Mesh->SetHiddenInGame(true);
	// 이 액터의 Collision을 없앤다.
	SetActorEnableCollision(false);

	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	// 이 액터를 없앤다.
	Destroy();
}
