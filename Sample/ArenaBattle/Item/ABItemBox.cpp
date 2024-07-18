// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Physics/ABCollision.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/ABCharacterItemInterface.h"
// �ּ� �Ŵ��� ����� ���� ���
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

	// ������ ����� ���Ҵ� (Ȥ�� ���� ����ų�) Ʈ���ſ� Collision Profile ���� ������.
	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// ���� �޽� ���
	ConstructorHelpers::FObjectFinder<UStaticMesh>BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	// �޽� ��ü�� �浹�� ����.
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	// ����Ʈ ���� ���� �ּ� ���
	ConstructorHelpers::FObjectFinder<UParticleSystem>BoxOpenEffectRef(TEXT("/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (BoxOpenEffectRef.Object)
	{
		// ��ƼŬ �ý����� SetTemplate �� ����
		Effect->SetTemplate(BoxOpenEffectRef.Object);
		// �������ڸ��� �ߵ��ϴ°� �ƴ϶�, �������� �� �ߵ��ؾ� ��.
		Effect->bAutoActivate = false;
	}
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// �ּ� �Ŵ����� �̱��� �ν��Ͻ��̹Ƿ� ������ �ʱ�ȭ�Ǹ� �ݵ�� �����Ѵ�.
	// �ּ� �Ŵ����� �ҷ��ͼ�
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	// PrimarytAssetIdList �� �޾ƿ´�.
	// �츮�� ������Ʈ ���ÿ��� ������ �±׸� (ABItemData) �� ������
	// ��� ������ �̷��� �±׸��� ������ �ִ� �ּµ��� ����� ��� ������ �ش�.
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	ensure(0 < Assets.Num());

	// �������� �ε��� ����
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	// �������� �ɾ �ε��� ���� ��������.
	// ���� ��θ� �����ͼ�
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	// ����ִ� UObject�� �ƴ϶��
	if (AssetPtr.IsPending())
	{
		// �ε����ش�.
		AssetPtr.LoadSynchronous();
	}
	// �ε��� �ּ��� �Ѱ��ش�.
	Item = Cast<UABItemData>(AssetPtr.Get());
	ensure(Item);

	// Trigger�� OnComponentBeginOverlap�� Dynamic���� ������ �� ��������Ʈ �̹Ƿ� �츮�� �ռ� ���� �Լ��� UFUNCTION()���� ������ �� ���̴�.
	// Spawn Ÿ�̹� ������ ���� PostInitializeComponent �� ����
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
}

// �������Ǹ� �ߵ��ϴ� �Լ�
void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// �������� ���ٸ� �׳� ����
	if (Item == nullptr)
	{
		Destroy();
		return;
	}

	// �������� ȹ���ϸ� ���Ϳ��� Take Item �Լ��� ȣ���ϵ��� ��Ų��.
	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	// ����Ʈ�� �ߵ��ϰ�
	Effect->Activate(true);
	// Mesh�� ���� ��
	Mesh->SetHiddenInGame(true);
	// �� ������ Collision�� ���ش�.
	SetActorEnableCollision(false);

	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	// �� ���͸� ���ش�.
	Destroy();
}
