// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//SetHp(MaxHp);
	// 이제 생성자에는 현재 레벨을 1로 초기화한다.
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;
}

// 액터, 컴포넌트, 위젯의 라이프사이클 상
// 캐릭터 스탯을 표시하는 위젯에게 정보를 넘기기 위해서는
// InitializeComponent 에서 액터의 스탯 정보가 미리 정의가 되어 있어야 함.
void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// 싱글톤의 최대 레벨과 1 사이에 반드시 레벨이 존재해야 한다.
	// 싱글톤 객체를 가져올 때에는 당근 Getter 로 가져와야 한다.
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	//BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
	// Deffered를 통해서 해결하자
	//CurrentHp = BaseStat.MaxHp;
}

float UABCharacterStatComponent::ApplyDamage(const float InDamage)
{
	const float PrevHp = CurrentHp;
	// 대미지가 음수인 경우 0으로 고정
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	// 현재 HP를 0~MaxHp 사이에서 이전값 - 실제 대미지로 한다.
	SetHp(PrevHp - ActualDamage);
	// HP가 최소값 이하이므로 죽게 되는 경우
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// 얘들아~~ 이놈 HP 제로란다~~
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

// 현재 HP를 설정하고 델리게이트 브로드캐스트함.
void UABCharacterStatComponent::SetHp(const float NewHp)
{
	//CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	// 이제 Stat을 사용한다.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	// 얘들아 지금 HP 이거란다~~
	OnHpChanged.Broadcast(CurrentHp);
}

