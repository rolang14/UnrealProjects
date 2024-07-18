// Copyright NHE


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"


AAuraCharacter::AAuraCharacter()
{
	// Camera Setting
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	SpringArm->SetupAttachment(RootComponent);

	// 이것들을 켜 놓으면 Parent (여기서는 Capsule Component) 의 회전을 유지하기 때문에, 카메라도 같이 돌아가버린다.
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	Camera->SetupAttachment(SpringArm);

	// Movement Setting for Top-Down RPG
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.0f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// 캐릭터 레벨 Getter (Interface)
int32 AAuraCharacter::GetCharacterLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	return AuraPlayerState->GetCharacterLevel();
}

// 서버에서만 호출된다.
// Possess 되었다는 것은.. 살아남았다는 것. 그니까 게임모드에 설정해 놓은 PlayerState 가 반드시 존재한다.
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Warning, TEXT("Hi Im Server"));

	// 어빌리티 정보를 세팅한다. (서버)
	SetupAbilityInfo();
	// For Startup Abilities (Server)
	AddCharacterAbilities();
}

// 클라이언트에서 Replicated PlayerState 를 받기 위함.
// 모든 Replicate Callback은 클라이언트에서만 호출된다.
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UE_LOG(LogTemp, Warning, TEXT("Hi Im Client"));

	// 어빌리티 정보를 세팅한다. (클라이언트)
	SetupAbilityInfo();
}

void AAuraCharacter::SetupAbilityInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerStateChecked<AAuraPlayerState>();
	// ActorInfo 설정. 직접적으로는 PlayerState 가 갖고 있으며 아바타는 이 캐릭터가 된다.
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	// Actor Info 설정 후에는 델리게이트 바인딩을 위해 함수 실행
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->SetupAbilitySystemComponent();

	this->AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	this->AttributeSet = AuraPlayerState->GetAttributeSet();

	// 여기서 많은 신규 개발자들이 PlayerController를 검사를 해야하는지 헷갈려 한다.
	// 여기선 if를 써야 되는가? assert를 써야 되는가? null 체크를 꼭 해야하는가? PlayerController(ex) 가 null 인 경우가 있을까?
	// 이것을 굉장히 많이 헷갈려한다.
	// 여기선 멀티플레이 환경이 기준이 되고, 우리는 계속 Replicate 를 신경써 왔다.
	// 우선 서버를 생각해 보면 모든 PlayerController 를 다 갖고 있다. 그러니 NULL이 될 일 자체가 없다.
	// 다음, 클라이언트를 생각해 보면 각 클라이언트는 자신의 PlayerController만 갖고 있지, 남의 PlayerController는 갖고 있지 않다.
	// 때문에 PlayerController 가 null 이 아닐때만 진행하면 된다는 흐름이 생긴다.
	// 따라서 이런 경우에는 if로 체크 후 진행하면 된다.
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// HUD도 마찬가지이다. LocalPlayer 일때만 HUD가 유효하다.
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Attribute 초기화를 서버에서 수행한다. 클라이언트에서는 이를 Replicate 하여 사용한다.
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Player Character Begin Play"));



}