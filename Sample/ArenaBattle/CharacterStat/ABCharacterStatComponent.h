// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// 델리게이트 선언
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
	// 델리게이트 구조체
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	// 레벨 설정
	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurLevel() const { return CurrentLevel; }

	// Base 스탯 추가 기능
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(BaseStat, ModifierStat); }

	// Base 스탯 설정, 델리게이트 브로드캐스트 추가
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InNewBaseStat) { BaseStat = InNewBaseStat; OnStatChanged.Broadcast(BaseStat, ModifierStat); }
	// Modifier 스탯 설정, 델리게이트 브로드캐스트 추가
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InNewModifierStat) { ModifierStat = InNewModifierStat; OnStatChanged.Broadcast(BaseStat, ModifierStat); }

	// Base, Modifier Stat Getter
	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }
	
	// 최종 스탯 설정
	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	// HP 회복 기능 (0 ~ Total MaxHp)
	FORCEINLINE void HealHp(const float HealAmount) { CurrentHp = FMath::Clamp(CurrentHp + HealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }

	FORCEINLINE float GetCurHp() const { return CurrentHp; }

	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// 대미지를 받은 경우, 대미지를 적용
	float ApplyDamage(const float InDamage);

// For Character Stat
protected:
	// 델리게이트와 바인딩될 함수
	// 반드시 이 함수를 통해 HP가 변경되어야 함.
	void SetHp(const float NewHp);

	// 보통 스탯 컴포넌트라는 오브젝트를 저장할 때, 이러한 속성들이 모두 다 디스크에 저장이 된다.
	// 근데 일단 여기서는 현재 HP값이 게임을 시작할때마다 새롭게 지정이 되기 때문에 굳이 디스크에 있을 필요가 없다.
	// 이럴 경우 Transient 라는 키워드를 선언해서 디스크에 불필요한 공간이 낭비되지 않도록 지정할 수 있다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 아래의 스탯들 역시, 게임 시작 시 데이터에 의해 초기화 되어야 하므로 Transient, 그리고
	// 수정이 되면 안되므로 에디터에서는 읽기 전용으로만 보이도록 설정한다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	// Attack Radius 추가
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// 게임 싱글톤으로부터 받아올 기본 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// 아아템 등에 의한 추가 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
