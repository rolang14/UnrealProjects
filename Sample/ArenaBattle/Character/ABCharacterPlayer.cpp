// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "CharacterStat/ABCharacterStatComponent.h"
#include "UI/ABHUDWidget.h"

#include "Interface/ABGameInterface.h"

#include "ABCharacterControlData.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// Camera
	// 컴포넌트 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	// Spring Arm 길이
	CameraBoom->TargetArmLength = 400.0f;
	// 실제로 카메라 지지대는 Pawn과 같이 돌아가지만
	CameraBoom->bUsePawnControlRotation = true;

	// 컴포넌트 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// 부착할 Parent와 뒤에는 부가적인 Socket이라는게 붙는다는데 뭔지 아직 잘 모르겠다. 그냥 USpringArmComponent의 소켓이라고 붙이나봄
	// 이렇게 하면 SpringArm의 끝쪽에 자동으로 붙는다고 함.
	// SocketName에 F12 눌러보면 SpringEndPoint 라고 아마 SprintArm에서 갖고있는 특정 정보인가봄.
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// 카메라 자체는 돌아가지 않는다. 카메라 지지대가 돌아갈 뿐이다.
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	// 액션 오브젝트 할당
	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/ArenaBattle/Input/IMC_Default.IMC_Default"));
	//if (InputMappingContextRef.Object)
	//{
	//	DefaultMappingContext = InputMappingContextRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move"));
	//if (InputActionMoveRef.Object)
	//{
	//	MoveAction = InputActionMoveRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look"));
	//if (InputActionLookRef.Object)
	//{
	//	LookAction = InputActionLookRef.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuarterMoveRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_QuarterMove.IA_QuarterMove"));
	if (InputActionQuarterMoveRef.Object)
	{
		QuarterMoveAction = InputActionQuarterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack"));
	if (InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quarter;

	//this->bUseControllerRotationPitch = true;
	//this->bUseControllerRotationYaw = true;
	//this->bUseControllerRotationRoll = true;
}

// CharacterControl을 변경
void AABCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quarter)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quarter);
	}
}

// 실제로는 여기서 컨트롤을 세팅함
void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// Base 에 TMap<ECharacterControlType, class UABCharacterControlData*> 으로 선언된 애가 데이터랑 갖고 있으므로
	UABCharacterControlData* NewCharacterControlData = CharacterControlManager[NewCharacterControlType];
	// 컨트롤 데이터가 반드시 있어야하므로
	check(NewCharacterControlData);
	// 그리고 걍 우리가 처음에 햇던 SetCharacterControlData 를 박아주자
	SetCharacterControlData(NewCharacterControlData);

	// 그리고 IMC 세팅
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	// 그리고 향상된 입력 시스템을 사용하기 위해 서브시스템을 가져온다. (걍 얘는 서브시스템 갖고와서 써야됨)
	// 대충 LocalPlayer 을 가져와서 그놈의 컨트롤러의 시스템에 맞게 해야하는거라서, GetSubSystem을 가져오는데, 이때 향상된 인풋을 쓸거니깐
	// 향상된 인풋에 맞는 LocalPlayerSubsystem을 가져온다. 가져오는거는 APlayerController 에서 GetLocalPlayer() 로 로컬 플레이어를 가져올 수 있다.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// 이전 매핑을 클리어하고
		Subsystem->ClearAllMappings();
		// 바뀌는 타입에 대한 매핑 컨텍스트를 가져와서
		UInputMappingContext* NewMappingContext = NewCharacterControlData->InputMappingContext;
		if (NewMappingContext)
		{
			// 서브시스템에 이거 쓰라고 갖다박는다
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	// 그리고 현재 컨트롤 타입을 지정.
	CurrentCharacterControlType = NewCharacterControlType;
}

// 컨트롤 데이터 세팅용
void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	// Input, SpringArm 에 대한 설정 추가
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

// 입력 설정해주는 함수.
// 넣어야되는건 EnhancedInputComponent 로 InputComponent 를 받아서, 액션과 함수를 바인딩해줘야한다.
void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent 를 설정하고, CastChecked 는 지정한 템플릿으로 캐스트가 되는지 체크하고,
	// 통과하지 못하면 에러를 발생시킨다. 즉 반드시 EnhancedInputComponent 만을 받기 위함이다.
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// EnhancedInputComponent 에다가 각종 액션을 바인딩한다.
	// Action 언리얼 오브젝트와 이후에 구현할 Jump, Move 등의 함수에 대해서 바인딩한다.
	// CharacterBase가 ACharacter를 상속한다는 것을 잊지 말자.
	// 때문에 Jump의 경우에는 ACharacter 에서 이미 제공하고있는 함수가 있으므로,
	// 아래는 지금 this 오브젝트에서 JumpAction이 트리거되었을 때 점프를 수행하고, 이벤트가 끝나면 Jump를 끝낸다는 식으로 구현한 것이다.
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// 그다음 이제 이거도 바꿔주자
	EnhancedInputComponent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuarterMove);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);

	// 그리고 추가된 액션을 첨가
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
}

void AABCharacterPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();
	UE_LOG(LogTemp, Log, TEXT("PawnClientRestart 가 시작하자마자 호출되네?!"));
}

// Quarter View Move
void AABCharacterPlayer::QuarterMove(const FInputActionValue& Value)
{
	// Value 에서 FVector2D를 가져온다.
	FVector2D MovementVector = Value.Get<FVector2d>();

	// QuarterMove 구현
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SizeSquared();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

// 이제 들어온 InputActionValue 를 통해 Move를 구현
void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	// Value 에서 FVector2D를 가져온다.
	FVector2D MovementVector = Value.Get<FVector2d>();

	// 컨트롤러에서 로테이션을 가져와서
	const FRotator Rotation = Controller->GetControlRotation();
	// Yaw 값을 사용
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Forward Direction 은 YawRotation의 X축으로,
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 측면 Direction 은 YawRotation의 Y로 가져온다..
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Movement 컴포넌트와 연결해서 실질적으로 Forward, 측면 방향으로 움직인다.
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Spring Arm 이 해당 컨트롤러를 바라보도록 설정
	// ControllerYawInput 으로 해당 Rotation 을 넣음.
	// 이걸 반대로 넣으면 마우스를 가로로 움직였을 때 카메라는 Pitch가 흔들리고,
	// 마우스를 세로로 움직였을 때 카메라는 Yaw가 흔들릴 것이다.
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

// HUD 셋업용
void AABCharacterPlayer::SetupHUDWidget(UABHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		// HUD의 각 위젯을 업데이트하기 위한 함수 호출
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurHp());

		// InHUDWidget 의 델리게이트 바인딩
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
	}
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetAliveState(true);

	// 이제는 IMC가 2개가 되면서 여기서 안하고 SetCharacterControl 에서 관리하니깐 거기서 하자
	/*
	// 해당 컨트롤러가 플레이어 전용 컨트롤러이기 때문에 CastChecked 를 사용한다.
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// 서브시스템에 매핑 컨텍스트를 연결하고 우선순위(0)을 지정한다.
		// 이렇게 우선순위를 지정하면 다양한 입력들이 서로 겹치더라도, 순위가 높은 입력들에 대해서
		// 액션들이 바인딩해서 수행할 수 있도록 지정이 가능한 것이다.
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		// 언제든지 런타임에서 매핑 컨텍스트를 삭제할 수 있다.
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
	*/

	// 그래서 여기서는 이제 BeginPlay() 시에 세팅하도록 함수 하나만 넣어주면 되는거
	SetCharacterControl(CurrentCharacterControlType);

	// Player 컨트롤러에 대한 Input 을 활성화한다.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
}

void AABCharacterPlayer::SetDead()
{
	Super::SetDead();

	// 플레이어가 죽은 경우 컨트롤러에서 더이상 입력받지 않도록 한다.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	IABGameInterface* ABGameMode = Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());
	if (ABGameMode)
	{
		ABGameMode->OnPlayerDead();
	}
}


