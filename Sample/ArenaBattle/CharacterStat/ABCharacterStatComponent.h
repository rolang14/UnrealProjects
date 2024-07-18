// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat& /*Base Stat*/, const FABCharacterStat& /*Modifier Stat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public :
	// ��������Ʈ ����ü
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	// ���� ����
	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurLevel() const { return CurrentLevel; }

	// Base ���� �߰� ���
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(BaseStat, ModifierStat); }

	// Base ���� ����, ��������Ʈ ��ε�ĳ��Ʈ �߰�
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InNewBaseStat) { BaseStat = InNewBaseStat; OnStatChanged.Broadcast(BaseStat, ModifierStat); }
	// Modifier ���� ����, ��������Ʈ ��ε�ĳ��Ʈ �߰�
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InNewModifierStat) { ModifierStat = InNewModifierStat; OnStatChanged.Broadcast(BaseStat, ModifierStat); }

	// Base, Modifier Stat Getter
	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }
	
	// ���� ���� ����
	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	// HP ȸ�� ��� (0 ~ Total MaxHp)
	FORCEINLINE void HealHp(const float HealAmount) { CurrentHp = FMath::Clamp(CurrentHp + HealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }

	FORCEINLINE float GetCurHp() const { return CurrentHp; }

	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// ������� ���� ���, ������� ����
	float ApplyDamage(const float InDamage);

// For Character Stat
protected:
	// ��������Ʈ�� ���ε��� �Լ�
	// �ݵ�� �� �Լ��� ���� HP�� ����Ǿ�� ��.
	void SetHp(const float NewHp);

	// ���� ���� ������Ʈ��� ������Ʈ�� ������ ��, �̷��� �Ӽ����� ��� �� ��ũ�� ������ �ȴ�.
	// �ٵ� �ϴ� ���⼭�� ���� HP���� ������ �����Ҷ����� ���Ӱ� ������ �Ǳ� ������ ���� ��ũ�� ���� �ʿ䰡 ����.
	// �̷� ��� Transient ��� Ű���带 �����ؼ� ��ũ�� ���ʿ��� ������ ������� �ʵ��� ������ �� �ִ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// �Ʒ��� ���ȵ� ����, ���� ���� �� �����Ϳ� ���� �ʱ�ȭ �Ǿ�� �ϹǷ� Transient, �׸���
	// ������ �Ǹ� �ȵǹǷ� �����Ϳ����� �б� �������θ� ���̵��� �����Ѵ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	// Attack Radius �߰�
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// ���� �̱������κ��� �޾ƿ� �⺻ ����
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// �ƾ��� � ���� �߰� ����
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
