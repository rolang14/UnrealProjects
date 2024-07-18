// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
// Capsule Component 에 대한 헤더
#include "Components/CapsuleComponent.h"
// Character Movement Component 에 대한 헤더 (대충 언리얼 DOCS 찾아보면 다 나온다)
#include "GameFramework/CharacterMovementComponent.h"
// 데이터 에셋 가져오기용
#include "ABCharacterControlData.h"
// 몽타주용
#include "Animation/AnimMontage.h"
// 콤보 데이터 애셋
#include "ABComboActionData.h"
// 공격 판정용 콜리전 정보
#include "Physics/ABCollision.h"
// 대미지 이벤트용
#include "Engine/DamageEvents.h"
// 캐릭터 스탯 컴포넌트용
#include "CharacterStat/ABCharacterStatComponent.h"
// 위젯 컴포넌트용 -> 확장하게 되면서 이제 필요없음
//#include "Components/WidgetComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABHpBarWidget.h"
// 이번엔 Weapon을 쓸거라서, 그리고 어차피 이걸 인클루드하면 Item까지 딸려옴.
//#include "Item/ABWeaponItemData.h"
// Item 을 한번에 INCLUDE
#include "Item/ABItems.h"

// 로그 카테고리 선언
DEFINE_LOG_CATEGORY(LogABCharacter);

// Sets default values
AABCharacterBase::AABCharacterBase()
{
	// Pawn
	// 카메라의 Pitch, Roll, Yaw에 대한 컨트롤 세팅
	// 이것들은 이제 폰이 컨트롤러의 Rotation Pitch, Roll, Yaw 값을 각각 따라갈 것인가를 의미한다.
	// 3인칭에서 뭐.. 카메라를 따라서 돌아가게 하고 싶으면 true 해도 되는데 보통은 그렇지 않으니 false로 한다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Capsule
	// 캡슐은 말 그대로 충돌 캡슐을 의미한다. (BP에서의)
	// 이 캡슐에 대한 설정
	// 아래처럼 해도 되지만 Character 에는 반드시 캡슐 컴포넌트가 존재하기 때문에 걍
	//UCapsuleComponent* CharacterCalsuleComponent = GetCapsuleComponent();
	//if (CharacterCalsuleComponent)
	//{
	//	CharacterCalsuleComponent->InitCapsuleSize(42.f, 96.0f);
	//	CharacterCalsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	//}

	// 일케 해주는게 더 간결하다
	// 순서대로 캡슐 사이즈를 정하고,
	// CollisionProfileName을 정한다. 이 충돌 프로필 이름은 나중에 뭔가랑 충돌했을때 이 이름을 가지고 비교하여
	// 특정 폰을 찾는 것이 가능하다.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	// Movement
	// true일 경우 캐릭터를 acceleration direction 에 맞게 회전시키며, 이 때 RotationRate에 따라 rotation이 바뀐다.
	// 뭔뜻이냐면, 우리가 입력하는 방향에 따라 자동으로 Character Mesh의 방향을 바꿔주는것이다. 회전보간도 다 해주기 때문에
	// 별도의 작업이 필요 없다. 다만 추가적인 입력-이동 매커니즘을 구현하려면 따로 정의해 주어야 한다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 회전율에 대한 부분인데 (회전 속도) Yaw 값이 올라갈수록 회전방향으로 더 빠르게 회전한다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	// Z축 점프 속도 (점프량)
	GetCharacterMovement()->JumpZVelocity = 700.f;
	// 캐릭터가 낙하중인 경우 캐릭터가 사용할 수 있는 측면 이동 제어량 (낙하중 양옆으로 움직이는것)
	GetCharacterMovement()->AirControl = 0.35f;
	// 최대 걷는 속도이며, 걷기의 경우 0부터 시작해서 점진적으로 Max까지 도달한다.
	// 또한, 이 역시 낙하중일 떄의 최대 측면 이동속도를 나타낸다는듯.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// 게임패드에서 이동키를 최고 적게 움직였을때 움직이는 속도 (즉 최소속도)
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	// 아무런 가속이 없을 때의 걷는 속도... 라는데 그냥 마찰력이다 ㅋㅋ
	// 마찰력이 2000.f 이므로 이보다 적은 힘을 받으면 미끌리지 않음. (0으로 하면 걍 아무 바닥에서 미끄러짐)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// 모델링 파일을 캐릭터의 진행방향에 맞춰서 설정해줘야한다. 순서대로 Location, Rotation.
	// 대충 Location 을 0.0f로 하면 아마 반쯤 땅에 묻혀있지 않을까? Rotation 역시 마찬가지.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	// 애니메이션 모드에서 컨트롤할 인터페이스를 정의함.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// 이것은 Mesh에 대한 Collision Name이다.
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// 그 다음은 이제 SkeletalMesh 를 부착해 주어야 한다. 마찬가지로 ConstructorHelpers 를 사용하여 Mesh를 찾아서 붙이자
	// 이건 Mesh를 갖고온다. 처음부터 SkeletalMesh 이므로 캐스팅 안해도 됨.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	// 이건 Animation 클래스를 갖고오므로 _C
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// 데이터 에셋으로 설정한 값 추가
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
	
	// 존재하는 블루프린트가 아니므로 서브오브젝트를 생성해 줘야 함.
	Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));
	// 위젯 컴포넌트
	HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
	// 위젯 컴포넌트의 위치 설정
	// 이 메시에다가
	HpBar->SetupAttachment(GetMesh());
	// 180.0f 만큼의 높이에
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// 위젯의 경우에는 애니메이션 블루프린트와 유사하게 클래스 정보를 등록해서, BeginPlay가 실행될 때
	// 클래스 정보로부터 인스턴스가 생성되는 형태로 구성이 된다.
	// 그러므로 우리가 만들어놓았던 HpBar를 넣기 위해서는 아래처럼 클래스 정보를 레퍼런스로부터 얻어와야 한다.
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		// 클래스 정보를 등록
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		// 3D가 아닌 2D 형태로 지정
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		// 위젯의 크기 : 가로, 세로
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		// 2D 위젯에 불필요한 부분 (콜리전 등) 처리
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
	// Pawn 섹션에 대한 데이터 에셋의 값을 받아옴
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// Character Movement Section
	// 마찬가지로 Character Movement 섹션에 대한 데이터 에셋의 값을 받아옴
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

// 어택 입력이 들어온 후의 콤보 진입 함수
void AABCharacterBase::ProcessComboCommand()
{
	// 콤보를 시작하지 않았다면
	if (CurrentCombo == 0)
	{
		// 콤보 액션을 시작한다.
		ComboActionBegin();
		// 콤보 진행중에는 여기로 진입하면 안됨.
		// 아래의 체크만 해야 한다.
		return;
	}

	// 타이머가 Valid하지 않을때는
	// 이미 해당 프레임이 지나서 타이머가 꺼졌거나
	// 더 이상 진행할 필요가 없을 경우임
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	// 타이머가 유효하다면 타이머가 돌고있을때 입력이 들어온 경우임
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
	// 이렇게 모드를 설정하면 움직이는 기능이 없어진다.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	// 애니메이션 재생 속도
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

	// 몽타주를 얻기 위해서는 AnimInstance를 가져와야 함. 이는 Skeletal Mesh 에 있다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// AnimInstance 에서 아래와 같이 몽타주를 플레이할 수 있다.
	// 기본속도는 1.0 이고 추가로 속도도 지정해 줄 수 있음.
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// 몽타주가 종료될 때 ComboActionEnd를 호출하고 싶음.
	// 그래서, 몽타주의 델리게이트를 사용하여 이벤트 지정함.

	// 그걸 위해 UAnimMontage.h 에서 보았던 델리게이트를 선언
	// 델리게이트 구조체 선언
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	// 종료 델리게이트를 설정함. 방금 만든 OnMontageEnded와, ComboActionEnd로 넘길 몽타주를 넣어준다.
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// 콤보가 시작될 때 타이머를 돌게 하기 위해
	// 타이머 핸들을 무효화(초기화)하고
	ComboTimerHandle.Invalidate();
	// 다시 타이머를 세팅
	SetComboCheckTimer();
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Log, TEXT("Combo Ended."));
	// 만약 ComboActionEnd가 호출되려면 CurrentCombo 가 0이어서는 안된다. 때문에 이거는 체크를 해줘야하므로
	// ensure ㄱㄱ (check : 강종, ensure : 에러로그 출력)
	ensure(CurrentCombo != 0);

	// 그리고 다시 0으로 만들어주고
	CurrentCombo = 0;
	// 다시 Walk가 가능하도록 해준다.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void AABCharacterBase::NotifyComboActionEnd()
{
}

void AABCharacterBase::SetComboCheckTimer()
{
	// 데이터 에셋에는 0~3에 1~4의 콤보가 들어있으므로 인덱스 설정용
	int32 ComboIndex = CurrentCombo - 1;
	// 계산한 콤보 인덱스가 올바른지 (만약 콤보가 3(2)까지밖에 없는데 인덱스가 3이 되면 이상한거잔아)
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	// 정상 속도 (1.0f)일 때 소요되는 시간 =
	// 17/30 프레임 / 속도 (2배면 1/2이니까)
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// 타이머를 발동시키려면 월드로부터 시간 서비스를 받아야 함
		// 타이머 핸들, 타이머 함수를 콜하는 객체, 타이머가 발동되면 호출할 함수, 타이머가 설정되고 발동하기까지의 시간(측정할 시간), 반복할것인지 한번만 할 것인지(Loop), (필요한 경우) 맨 처음 인터벌 부가설정
		// 타이머는 17/30초 후에 ComboCheck를 호출한다.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

// 타이머가 발동되어 호출되면
void AABCharacterBase::ComboCheck()
{
	// 더이상 발동되지 않도록 타이머 해제
	ComboTimerHandle.Invalidate();
	// HasNextComboCommand : 17/30초 이내에 입력이 있었다면 ProcessComboCommand()에 의해 true가 된다.
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		// Clamp : 특정 값을 Min ~ Max 사이에서 벗어나지 않는 조건으로 변화시킴
		// 여튼 다음 콤보 단계를 갱신하고
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		UE_LOG(LogTemp, Log, TEXT("Next Combo : %d"), CurrentCombo);
		// 다음 애니메이션 재생을 위해 몽타주 섹션 이름을 정해줄건데 우리는 앞서 ComboAttack1~4로 정의했다.
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		// 해당 섹션으로 몽타주가 바로 점프됨
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		// 그리고 다시 바로 타이머를 걸어줌
		SetComboCheckTimer();
		// 다시 체크값 초기화
		HasNextComboCommand = false;
	}
}

// Trace를 사용하여 충돌하는지 검사
void AABCharacterBase::AttackHitCheck()
{
	// Hit 상태를 구성하는 구조체
	FHitResult OutHitResult;
	// Collision 쿼리를 구성하는 파라미터.
	// 앞에서부터 순서대로, TraceTag는 나중에 어떤 태그 정보를 기반으로 분석할 때의 식별자 정보로써 사용되는 인자. 여기에서는 공격에 대한 판정이므로 Attack 이라고 걍 넣어준거고 이렇게 넣으면 SCENE_QUERY_STAT 가 알아서 해준다.
	// 그 다음은 InTraceComplex 인데, 단순한 형태인지, 복잡한 형태의 물리 계산도 추가할 것인지에 대한 판단.
	// 마지막은 충돌에서 제외할 액터이다. 자기자신을 제외하면 되므로 this를 넣어줌.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 공격 범위
	const float AttackRange = Stat->GetTotalStat().AttackRange;
	// 구체를 만들어 스윕 검사를 진행할 것이므로 해당 구체의 Rad
	const float AttackRadius = Stat->GetAttackRadius();
	// 대미지
	const float AttackDamage = Stat->GetTotalStat().Attack;
	// 검사 범위
	// 검사 시작 위치
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	// 검사 종료 위치
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// Tekla의 Intersect 라고 보면 편하고,
	// Sweep을 사용해서 Channel로 검사를 할건데
	// Hit Result를 설정하고, 검사 시작, 종료 위치를 설정 후, 몬진 모르겠지만 쿼터니언을 기준으로 계산한다는듯,
	// 그리고 미리 지정한 ABAction 채널로 검사할 것이고, FCollisionShape 를 사용하여 스피어를 만들어 계산한다.
	// 마지막으로 Collision 검사에 사용할 파라미터를 넣는다.
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		// 무언가가 히트되었다고 감지한 것.
		FDamageEvent DamageEvent;
		// OutHitResult 에 들어있는 액터(공격을 당한 액터) 에 TakeDamage를 발생시킨다.
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	// 디버깅을 위해 Capsule을 그린다.
	// 공격 범위 Capsule (Sphere가 Sweep 되었으므로 Capsule 모양이어야 함)의 중심은 Start에서 End <-> Start 거리만큼의 반을 가면 된다.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	// Capsule Half Height 는 Capsule의 절반 높이에서 Rad만큼 뺀 값이다 (직선높이만을 의미함)
	float CapsuleHalfHeight = AttackRange * 0.5f;
	// 캡슐 색 지정
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	// 디버그 캡슐을 그리는 함수.
	// 월드에, Origin 에서, HalfHeight, Radius를 갖는 캡슐을 그리는데,
	// Z축이 위가 아니라, 시선 방향을 향햐도록 바꾸어 주고 (누운 캡슐 모양)
	// 정해진 색과, 선이 계속 유지될 것인지, 유지되지 않는다면 몇초(LifeCycle) 동안 표시될 것인지를 의미한다. (5초)
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

// 리턴은 최종으로 실제 받은 대미지 양을 반환한다.
// 순서대로 액터에게 적용할 대미지 양, 받은 대미지에 대해 묘사할 대미지 패키지(대미지 이벤트), 대미지 이벤트를 일으킨 컨트롤러(가해자), 대미지를 준 액터(가해자의 도구)
float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCause);
	// 실제로는 이제 여기에 대미지를 계산하는 공식이 들어간다.
	// 지금은 일단 죽는 모션을 재생하도록 하자.
	//SetDead();
	// 이제 그냥 죽는게 아니라 대미지를 입도록 변경한다.
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

// 죽었을 때 해줘야 할 세팅은 여러가지가 있을거임.
// 예를 들어 Movement를 제한한다거나, 조작이 안되게 한다거나 등.
void AABCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	// 죽었으므로 Collision 기능을 꺼 준다.
	// 죽었는데 계속 대미지를 받고 계속 죽으면 이상하니깐
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	SetAliveState(false);
}

// 죽는 모션 재생
void AABCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 재생중인 모든 모션 중지. 파라미터는 BlendOut 시간임
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadActionMontage, 1.0f);
}

void AABCharacterBase::SetUpCharacterWidget(UABUserWidget* InUserWidget)
{
	// Hp Bar 위젯 설정 추가
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
		// InItemData의 타입을(열거형) uint8로 캐스팅하여
		// 아이템 종류 델리게이트인 인덱스로 사용할 수 있다.
		// 그리고 그 인덱스의 델리게이트 바운딩된 함수를 실행한다. 이는 생성자에서 바운드해놓았다.
		// 그리고 이렇게 사용하려면 반드시 델리게이트 래퍼 배열에 바운딩한 순서와
		// Item의 타입 ENUM 순서를 같이 해야함.
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Equip Weapon"));

	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);
	if (InItemData)
	{
		// WeaponMesh가 아직 로딩이 안되었다면
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			// WeaponMesh를 동기적으로 로드하고
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		// 직접적으로 참조하는것이 아니라, Get을 통해 Mesh를 간접적으로 가져온다.
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

