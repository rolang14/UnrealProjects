// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//SetHp(MaxHp);
	// ���� �����ڿ��� ���� ������ 1�� �ʱ�ȭ�Ѵ�.
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;
}

// ����, ������Ʈ, ������ ����������Ŭ ��
// ĳ���� ������ ǥ���ϴ� �������� ������ �ѱ�� ���ؼ���
// InitializeComponent ���� ������ ���� ������ �̸� ���ǰ� �Ǿ� �־�� ��.
void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// �̱����� �ִ� ������ 1 ���̿� �ݵ�� ������ �����ؾ� �Ѵ�.
	// �̱��� ��ü�� ������ ������ ��� Getter �� �����;� �Ѵ�.
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	//BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
	// Deffered�� ���ؼ� �ذ�����
	//CurrentHp = BaseStat.MaxHp;
}

float UABCharacterStatComponent::ApplyDamage(const float InDamage)
{
	const float PrevHp = CurrentHp;
	// ������� ������ ��� 0���� ����
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	// ���� HP�� 0~MaxHp ���̿��� ������ - ���� ������� �Ѵ�.
	SetHp(PrevHp - ActualDamage);
	// HP�� �ּҰ� �����̹Ƿ� �װ� �Ǵ� ���
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// ����~~ �̳� HP ���ζ���~~
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

// ���� HP�� �����ϰ� ��������Ʈ ��ε�ĳ��Ʈ��.
void UABCharacterStatComponent::SetHp(const float NewHp)
{
	//CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	// ���� Stat�� ����Ѵ�.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	// ���� ���� HP �̰Ŷ���~~
	OnHpChanged.Broadcast(CurrentHp);
}

