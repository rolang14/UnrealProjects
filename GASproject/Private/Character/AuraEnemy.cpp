// Copyright NHE


#include "Character/AuraEnemy.h"
#include "Interaction/AuraCustomDepthValule.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "UI/Widget/AuraUserWidget.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AAuraEnemy::AAuraEnemy()
{
	bIsHitReacting = false;

	// Visibility Channel 을 Block 하도록 변경
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Enemy에서는 캐릭터 자체에 Ability를 부여한다.
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

#pragma region Highlight Enemy Interface

void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Highlighted"));

	// 그냥 땡으로 true 250을 넣어버리면, 렌더 상태를 Dirty 상태로 직접 만들어서 렌더 옵션을 변경해야 한다.
	// 그러므로 그냥 SetRenderCustomDepth 를 사용하는게 낫다.
	//GetMesh()->bRenderCustomDepth = true;
	//GetMesh()->CustomDepthStencilValue = 250;
	//GetMesh()->MarkRenderStateDirty();

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED_OUTLINE);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED_OUTLINE);
}

void AAuraEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("UnHighlighted"));

	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_NONE);

	Weapon->SetRenderCustomDepth(false);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_NONE);
}

int32 AAuraEnemy::GetCharacterLevel()
{
	return Level;
}

void AAuraEnemy::PerformDeath()
{
	SetLifeSpan(DeathLifeSpan);
	Super::PerformDeath();
}

#pragma endregion

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	// For Setting up Ability System Component and Attribute Set
	if (HasAuthority())
	{
		SetupAbilityInfo();
		
		// For Enemy Startup Abilities
		AddCharacterAbilities();
	}

	// For Attribute Delegate to HealthWidget
	SetupHealthWidgetComponent();
}

void AAuraEnemy::SetupAbilityInfo()
{
	check(AbilitySystemComponent);
	check(AttributeSet);

	// AI가 컨트롤하는 캐릭터는 그냥 BeginPlay 에서 ActorInfo를 초기화해도 무방하다.
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->SetupAbilitySystemComponent();
	InitializeDefaultAttributes();

	// Gameplay Effect 가 부여하는 Tag 변화에 반응하기 위함
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, AbilitySystemComponent, CharacterClass, Level);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewTagCount)
{
	// 현재 HitReacting 인지 아는 것은 굉장히 중요하다. 여러 행동이라던지, AI Behavior Tree 에서도 이에 따라 많은 행동이 갈린다.
	// 이 콜백은 설정한 Tag, 즉 Effects_HitReact 가 NewOrRemoved 되었을 때만 호출되므로 HitReact 중이 아니라면 일반적인 움직임으로 되돌려야 한다.
	// 때문에 bIsHitReacting 를 멤버로 갖는 것이 좋다. 
	bIsHitReacting = NewTagCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::AddCharacterAbilities()
{
	UAuraAbilitySystemLibrary::GiveStarupAbilities(this, AbilitySystemComponent);
}

void AAuraEnemy::SetupHealthWidgetComponent()
{
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	// 다른 AttributeSet 을 사용한다면 CastChecked 를 변경해야 함.
	// Binding Delegates
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Broadcast Initial Value
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}
