// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
// Capsule Component �� ���� ���
#include "Components/CapsuleComponent.h"
// Character Movement Component �� ���� ��� (���� �𸮾� DOCS ã�ƺ��� �� ���´�)
#include "GameFramework/CharacterMovementComponent.h"
// ������ ���� ���������
#include "ABCharacterControlData.h"
// ��Ÿ�ֿ�
#include "Animation/AnimMontage.h"
// �޺� ������ �ּ�
#include "ABComboActionData.h"
// ���� ������ �ݸ��� ����
#include "Physics/ABCollision.h"
// ����� �̺�Ʈ��
#include "Engine/DamageEvents.h"
// ĳ���� ���� ������Ʈ��
#include "CharacterStat/ABCharacterStatComponent.h"
// ���� ������Ʈ�� -> Ȯ���ϰ� �Ǹ鼭 ���� �ʿ����
//#include "Components/WidgetComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABHpBarWidget.h"
// �̹��� Weapon�� ���Ŷ�, �׸��� ������ �̰� ��Ŭ����ϸ� Item���� ������.
//#include "Item/ABWeaponItemData.h"
// Item �� �ѹ��� INCLUDE
#include "Item/ABItems.h"

// �α� ī�װ� ����
DEFINE_LOG_CATEGORY(LogABCharacter);

// Sets default values
AABCharacterBase::AABCharacterBase()
{
	// Pawn
	// ī�޶��� Pitch, Roll, Yaw�� ���� ��Ʈ�� ����
	// �̰͵��� ���� ���� ��Ʈ�ѷ��� Rotation Pitch, Roll, Yaw ���� ���� ���� ���ΰ��� �ǹ��Ѵ�.
	// 3��Ī���� ��.. ī�޶� ���� ���ư��� �ϰ� ������ true �ص� �Ǵµ� ������ �׷��� ������ false�� �Ѵ�.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Capsule
	// ĸ���� �� �״�� �浹 ĸ���� �ǹ��Ѵ�. (BP������)
	// �� ĸ���� ���� ����
	// �Ʒ�ó�� �ص� ������ Character ���� �ݵ�� ĸ�� ������Ʈ�� �����ϱ� ������ ��
	//UCapsuleComponent* CharacterCalsuleComponent = GetCapsuleComponent();
	//if (CharacterCalsuleComponent)
	//{
	//	CharacterCalsuleComponent->InitCapsuleSize(42.f, 96.0f);
	//	CharacterCalsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	//}

	// ���� ���ִ°� �� �����ϴ�
	// ������� ĸ�� ����� ���ϰ�,
	// CollisionProfileName�� ���Ѵ�. �� �浹 ������ �̸��� ���߿� ������ �浹������ �� �̸��� ������ ���Ͽ�
	// Ư�� ���� ã�� ���� �����ϴ�.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	// Movement
	// true�� ��� ĳ���͸� acceleration direction �� �°� ȸ����Ű��, �� �� RotationRate�� ���� rotation�� �ٲ��.
	// �����̳ĸ�, �츮�� �Է��ϴ� ���⿡ ���� �ڵ����� Character Mesh�� ������ �ٲ��ִ°��̴�. ȸ�������� �� ���ֱ� ������
	// ������ �۾��� �ʿ� ����. �ٸ� �߰����� �Է�-�̵� ��Ŀ������ �����Ϸ��� ���� ������ �־�� �Ѵ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// ȸ������ ���� �κ��ε� (ȸ�� �ӵ�) Yaw ���� �ö󰥼��� ȸ���������� �� ������ ȸ���Ѵ�.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	// Z�� ���� �ӵ� (������)
	GetCharacterMovement()->JumpZVelocity = 700.f;
	// ĳ���Ͱ� �������� ��� ĳ���Ͱ� ����� �� �ִ� ���� �̵� ��� (������ �翷���� �����̴°�)
	GetCharacterMovement()->AirControl = 0.35f;
	// �ִ� �ȴ� �ӵ��̸�, �ȱ��� ��� 0���� �����ؼ� ���������� Max���� �����Ѵ�.
	// ����, �� ���� �������� ���� �ִ� ���� �̵��ӵ��� ��Ÿ���ٴµ�.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// �����е忡�� �̵�Ű�� �ְ� ���� ���������� �����̴� �ӵ� (�� �ּҼӵ�)
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	// �ƹ��� ������ ���� ���� �ȴ� �ӵ�... ��µ� �׳� �������̴� ����
	// �������� 2000.f �̹Ƿ� �̺��� ���� ���� ������ �̲����� ����. (0���� �ϸ� �� �ƹ� �ٴڿ��� �̲�����)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// �𵨸� ������ ĳ������ ������⿡ ���缭 ����������Ѵ�. ������� Location, Rotation.
	// ���� Location �� 0.0f�� �ϸ� �Ƹ� ���� ���� �������� ������? Rotation ���� ��������.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	// �ִϸ��̼� ��忡�� ��Ʈ���� �������̽��� ������.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// �̰��� Mesh�� ���� Collision Name�̴�.
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// �� ������ ���� SkeletalMesh �� ������ �־�� �Ѵ�. ���������� ConstructorHelpers �� ����Ͽ� Mesh�� ã�Ƽ� ������
	// �̰� Mesh�� ����´�. ó������ SkeletalMesh �̹Ƿ� ĳ���� ���ص� ��.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	// �̰� Animation Ŭ������ ������Ƿ� _C
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// ������ �������� ������ �� �߰�
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add({ ECharacterControlType::Shoulder, ShoulderDataRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuarterDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quarter.ABC_Quarter"));
	if (QuarterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quarter, QuarterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_Combo_Attack.AM_Combo_Attack"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead"));
	if (DeadActionMontageRef.Object)
	{
		DeadActionMontage = DeadActionMontageRef.Object;
	}
	
	// �����ϴ� �������Ʈ�� �ƴϹǷ� ���������Ʈ�� ������ ��� ��.
	Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));
	// ���� ������Ʈ
	HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
	// ���� ������Ʈ�� ��ġ ����
	// �� �޽ÿ��ٰ�
	HpBar->SetupAttachment(GetMesh());
	// 180.0f ��ŭ�� ���̿�
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// ������ ��쿡�� �ִϸ��̼� �������Ʈ�� �����ϰ� Ŭ���� ������ ����ؼ�, BeginPlay�� ����� ��
	// Ŭ���� �����κ��� �ν��Ͻ��� �����Ǵ� ���·� ������ �ȴ�.
	// �׷��Ƿ� �츮�� �������Ҵ� HpBar�� �ֱ� ���ؼ��� �Ʒ�ó�� Ŭ���� ������ ���۷����κ��� ���;� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		// Ŭ���� ������ ���
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		// 3D�� �ƴ� 2D ���·� ����
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		// ������ ũ�� : ����, ����
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		// 2D ������ ���ʿ��� �κ� (�ݸ��� ��) ó��
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Item Action
	TakeItemActions.Emplace(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
	TakeItemActions.Emplace(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPotion)));
	TakeItemActions.Emplace(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));

	// Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AABCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AABCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AABCharacterBase::ApplyStat);
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	// Pawn Section
	// Pawn ���ǿ� ���� ������ ������ ���� �޾ƿ�
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// Character Movement Section
	// ���������� Character Movement ���ǿ� ���� ������ ������ ���� �޾ƿ�
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

// ���� �Է��� ���� ���� �޺� ���� �Լ�
void AABCharacterBase::ProcessComboCommand()
{
	// �޺��� �������� �ʾҴٸ�
	if (CurrentCombo == 0)
	{
		// �޺� �׼��� �����Ѵ�.
		ComboActionBegin();
		// �޺� �����߿��� ����� �����ϸ� �ȵ�.
		// �Ʒ��� üũ�� �ؾ� �Ѵ�.
		return;
	}

	// Ÿ�̸Ӱ� Valid���� ��������
	// �̹� �ش� �������� ������ Ÿ�̸Ӱ� �����ų�
	// �� �̻� ������ �ʿ䰡 ���� �����
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	// Ÿ�̸Ӱ� ��ȿ�ϴٸ� Ÿ�̸Ӱ� ���������� �Է��� ���� �����
	else
	{
		HasNextComboCommand = true;
	}
}

void AABCharacterBase::ComboActionBegin()
{
	// Combo Begins
	CurrentCombo = 1;

	// Movement Setting
	// �̷��� ��带 �����ϸ� �����̴� ����� ��������.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	// �ִϸ��̼� ��� �ӵ�
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

	// ��Ÿ�ָ� ��� ���ؼ��� AnimInstance�� �����;� ��. �̴� Skeletal Mesh �� �ִ�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// AnimInstance ���� �Ʒ��� ���� ��Ÿ�ָ� �÷����� �� �ִ�.
	// �⺻�ӵ��� 1.0 �̰� �߰��� �ӵ��� ������ �� �� ����.
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// ��Ÿ�ְ� ����� �� ComboActionEnd�� ȣ���ϰ� ����.
	// �׷���, ��Ÿ���� ��������Ʈ�� ����Ͽ� �̺�Ʈ ������.

	// �װ� ���� UAnimMontage.h ���� ���Ҵ� ��������Ʈ�� ����
	// ��������Ʈ ����ü ����
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	// ���� ��������Ʈ�� ������. ��� ���� OnMontageEnded��, ComboActionEnd�� �ѱ� ��Ÿ�ָ� �־��ش�.
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// �޺��� ���۵� �� Ÿ�̸Ӹ� ���� �ϱ� ����
	// Ÿ�̸� �ڵ��� ��ȿȭ(�ʱ�ȭ)�ϰ�
	ComboTimerHandle.Invalidate();
	// �ٽ� Ÿ�̸Ӹ� ����
	SetComboCheckTimer();
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Log, TEXT("Combo Ended."));
	// ���� ComboActionEnd�� ȣ��Ƿ��� CurrentCombo �� 0�̾�� �ȵȴ�. ������ �̰Ŵ� üũ�� ������ϹǷ�
	// ensure ���� (check : ����, ensure : �����α� ���)
	ensure(CurrentCombo != 0);

	// �׸��� �ٽ� 0���� ������ְ�
	CurrentCombo = 0;
	// �ٽ� Walk�� �����ϵ��� ���ش�.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void AABCharacterBase::NotifyComboActionEnd()
{
}

void AABCharacterBase::SetComboCheckTimer()
{
	// ������ ���¿��� 0~3�� 1~4�� �޺��� ��������Ƿ� �ε��� ������
	int32 ComboIndex = CurrentCombo - 1;
	// ����� �޺� �ε����� �ùٸ��� (���� �޺��� 3(2)�����ۿ� ���µ� �ε����� 3�� �Ǹ� �̻��Ѱ��ܾ�)
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	// ���� �ӵ� (1.0f)�� �� �ҿ�Ǵ� �ð� =
	// 17/30 ������ / �ӵ� (2��� 1/2�̴ϱ�)
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// Ÿ�̸Ӹ� �ߵ���Ű���� ����κ��� �ð� ���񽺸� �޾ƾ� ��
		// Ÿ�̸� �ڵ�, Ÿ�̸� �Լ��� ���ϴ� ��ü, Ÿ�̸Ӱ� �ߵ��Ǹ� ȣ���� �Լ�, Ÿ�̸Ӱ� �����ǰ� �ߵ��ϱ������ �ð�(������ �ð�), �ݺ��Ұ����� �ѹ��� �� ������(Loop), (�ʿ��� ���) �� ó�� ���͹� �ΰ�����
		// Ÿ�̸Ӵ� 17/30�� �Ŀ� ComboCheck�� ȣ���Ѵ�.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

// Ÿ�̸Ӱ� �ߵ��Ǿ� ȣ��Ǹ�
void AABCharacterBase::ComboCheck()
{
	// ���̻� �ߵ����� �ʵ��� Ÿ�̸� ����
	ComboTimerHandle.Invalidate();
	// HasNextComboCommand : 17/30�� �̳��� �Է��� �־��ٸ� ProcessComboCommand()�� ���� true�� �ȴ�.
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		// Clamp : Ư�� ���� Min ~ Max ���̿��� ����� �ʴ� �������� ��ȭ��Ŵ
		// ��ư ���� �޺� �ܰ踦 �����ϰ�
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		UE_LOG(LogTemp, Log, TEXT("Next Combo : %d"), CurrentCombo);
		// ���� �ִϸ��̼� ����� ���� ��Ÿ�� ���� �̸��� �����ٰǵ� �츮�� �ռ� ComboAttack1~4�� �����ߴ�.
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		// �ش� �������� ��Ÿ�ְ� �ٷ� ������
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		// �׸��� �ٽ� �ٷ� Ÿ�̸Ӹ� �ɾ���
		SetComboCheckTimer();
		// �ٽ� üũ�� �ʱ�ȭ
		HasNextComboCommand = false;
	}
}

// Trace�� ����Ͽ� �浹�ϴ��� �˻�
void AABCharacterBase::AttackHitCheck()
{
	// Hit ���¸� �����ϴ� ����ü
	FHitResult OutHitResult;
	// Collision ������ �����ϴ� �Ķ����.
	// �տ������� �������, TraceTag�� ���߿� � �±� ������ ������� �м��� ���� �ĺ��� �����ν� ���Ǵ� ����. ���⿡���� ���ݿ� ���� �����̹Ƿ� Attack �̶�� �� �־��ذŰ� �̷��� ������ SCENE_QUERY_STAT �� �˾Ƽ� ���ش�.
	// �� ������ InTraceComplex �ε�, �ܼ��� ��������, ������ ������ ���� ��굵 �߰��� �������� ���� �Ǵ�.
	// �������� �浹���� ������ �����̴�. �ڱ��ڽ��� �����ϸ� �ǹǷ� this�� �־���.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// ���� ����
	const float AttackRange = Stat->GetTotalStat().AttackRange;
	// ��ü�� ����� ���� �˻縦 ������ ���̹Ƿ� �ش� ��ü�� Rad
	const float AttackRadius = Stat->GetAttackRadius();
	// �����
	const float AttackDamage = Stat->GetTotalStat().Attack;
	// �˻� ����
	// �˻� ���� ��ġ
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	// �˻� ���� ��ġ
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// Tekla�� Intersect ��� ���� ���ϰ�,
	// Sweep�� ����ؼ� Channel�� �˻縦 �Ұǵ�
	// Hit Result�� �����ϰ�, �˻� ����, ���� ��ġ�� ���� ��, ���� �𸣰����� ���ʹϾ��� �������� ����Ѵٴµ�,
	// �׸��� �̸� ������ ABAction ä�η� �˻��� ���̰�, FCollisionShape �� ����Ͽ� ���Ǿ ����� ����Ѵ�.
	// ���������� Collision �˻翡 ����� �Ķ���͸� �ִ´�.
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		// ���𰡰� ��Ʈ�Ǿ��ٰ� ������ ��.
		FDamageEvent DamageEvent;
		// OutHitResult �� ����ִ� ����(������ ���� ����) �� TakeDamage�� �߻���Ų��.
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	// ������� ���� Capsule�� �׸���.
	// ���� ���� Capsule (Sphere�� Sweep �Ǿ����Ƿ� Capsule ����̾�� ��)�� �߽��� Start���� End <-> Start �Ÿ���ŭ�� ���� ���� �ȴ�.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	// Capsule Half Height �� Capsule�� ���� ���̿��� Rad��ŭ �� ���̴� (�������̸��� �ǹ���)
	float CapsuleHalfHeight = AttackRange * 0.5f;
	// ĸ�� �� ����
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	// ����� ĸ���� �׸��� �Լ�.
	// ���忡, Origin ����, HalfHeight, Radius�� ���� ĸ���� �׸��µ�,
	// Z���� ���� �ƴ϶�, �ü� ������ ���ᵵ�� �ٲپ� �ְ� (���� ĸ�� ���)
	// ������ ����, ���� ��� ������ ������, �������� �ʴ´ٸ� ����(LifeCycle) ���� ǥ�õ� �������� �ǹ��Ѵ�. (5��)
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

// ������ �������� ���� ���� ����� ���� ��ȯ�Ѵ�.
// ������� ���Ϳ��� ������ ����� ��, ���� ������� ���� ������ ����� ��Ű��(����� �̺�Ʈ), ����� �̺�Ʈ�� ����Ų ��Ʈ�ѷ�(������), ������� �� ����(�������� ����)
float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCause);
	// �����δ� ���� ���⿡ ������� ����ϴ� ������ ����.
	// ������ �ϴ� �״� ����� ����ϵ��� ����.
	//SetDead();
	// ���� �׳� �״°� �ƴ϶� ������� �Ե��� �����Ѵ�.
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

// �׾��� �� ����� �� ������ ���������� ��������.
// ���� ��� Movement�� �����Ѵٰų�, ������ �ȵǰ� �Ѵٰų� ��.
void AABCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	// �׾����Ƿ� Collision ����� �� �ش�.
	// �׾��µ� ��� ������� �ް� ��� ������ �̻��ϴϱ�
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	SetAliveState(false);
}

// �״� ��� ���
void AABCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// ������� ��� ��� ����. �Ķ���ʹ� BlendOut �ð���
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadActionMontage, 1.0f);
}

void AABCharacterBase::SetUpCharacterWidget(UABUserWidget* InUserWidget)
{
	// Hp Bar ���� ���� �߰�
	UABHpBarWidget* HpBarWidget = Cast<UABHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		Stat->OnStatChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateStat);
		HpBarWidget->UpdateHpBar(Stat->GetCurHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
	}
}

void AABCharacterBase::TakeItem(UABItemData* InItemData)
{
	if (InItemData)
	{
		// InItemData�� Ÿ����(������) uint8�� ĳ�����Ͽ�
		// ������ ���� ��������Ʈ�� �ε����� ����� �� �ִ�.
		// �׸��� �� �ε����� ��������Ʈ �ٿ���� �Լ��� �����Ѵ�. �̴� �����ڿ��� �ٿ���س��Ҵ�.
		// �׸��� �̷��� ����Ϸ��� �ݵ�� ��������Ʈ ���� �迭�� �ٿ���� ������
		// Item�� Ÿ�� ENUM ������ ���� �ؾ���.
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Equip Weapon"));

	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);
	if (InItemData)
	{
		// WeaponMesh�� ���� �ε��� �ȵǾ��ٸ�
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			// WeaponMesh�� ���������� �ε��ϰ�
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		// ���������� �����ϴ°��� �ƴ϶�, Get�� ���� Mesh�� ���������� �����´�.
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
}

void AABCharacterBase::DrinkPotion(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Drink Potion"));

	UABPotionItemData* PotionItemData = Cast<UABPotionItemData>(InItemData);
	if (PotionItemData)
	{
		Stat->HealHp(PotionItemData->HealAmount);
	}
}

void AABCharacterBase::ReadScroll(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Read Scroll"));

	UABScrollItemData* ScrollItemData = Cast<UABScrollItemData>(InItemData);
	if (ScrollItemData)
	{
		Stat->AddBaseStat(ScrollItemData->BaseStat);
	}
}

const int32 AABCharacterBase::GetLevel() const
{
	return Stat->GetCurLevel();
}

void AABCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}

void AABCharacterBase::ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	float TotalMovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = TotalMovementSpeed;
}

const bool AABCharacterBase::CharacterAliveCheck() const
{
	return GetAliveState();
}

