// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RBCharacterStat.h"
#include "RBCharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRB_API URBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URBCharacterStatComponent();

	// Stat Getter & Setter
	FORCEINLINE const FRBCharacterStat GetTotalStat() const { return BaseStat + AdditionalStat; }

	FORCEINLINE const float& GetFieldAttackRadius() const { return FieldAttackRadius; }
	FORCEINLINE const float& GetFieldAttackRange() const { return FieldAttackRange; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// 현재 HP
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat")
	float CurrentHp;

	// 필드 어택 반경
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float FieldAttackRadius;

	// 필드 어택 사거리
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float FieldAttackRange;

	// 피버 어택 사거리
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float FeverAttackRange;

	// 캐릭터의 기본 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat")
	FRBCharacterStat BaseStat;

	// 상태변화, 장비 등에 의한 추가 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat")
	FRBCharacterStat AdditionalStat;
};
