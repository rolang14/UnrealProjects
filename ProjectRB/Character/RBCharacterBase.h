// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/RBCharacterFieldAttackInterface.h"
#include "RBCharacterBase.generated.h"


class ARBWeaponTypeBase;
class URBCharacterStatComponent;


UCLASS(Abstract)
class PROJECTRB_API ARBCharacterBase : public ACharacter, public IRBCharacterFieldAttackInterface
{
	GENERATED_BODY()

public:
	ARBCharacterBase();

// Get Character Status
public:
	// 캐릭터의 무기 정보를 취득
	FORCEINLINE const TObjectPtr<ARBWeaponTypeBase> GetCharacterWeapon() const { return Weapon; }

	// 캐릭터 이름/코드 취득 (내부적으로 캐릭터 구분용)
	FORCEINLINE const FName GetCharacterName() const { return CharacterName; }

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

//public:
//	UPROPERTY(VisibleInstanceOnly, Category = "Character Name")
//	FString Name;

// Character Stat Section
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URBCharacterStatComponent> Stat;

#pragma region Movement

public:
	UPROPERTY(BlueprintReadOnly, Category = "Character Turn In Place")
	float DesiredYaw = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Character Turn In Place")
	void TurnTo(const FRotator DesiredRotation);

protected:
	FRotator TargetRotation;
	bool IsTurning;

#pragma endregion

#pragma region Field Attack

protected:
	// 필드 어택 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Character Motion")
	TObjectPtr<UAnimMontage> FieldAttackMontage;

	// 필드 어택 피격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Character Motion")
	TObjectPtr<UAnimMontage> FieldAttackHitMontage;

	// Perform Field Attack
	void FieldAttack();

	FORCEINLINE const bool& IsProcessingFieldAttack() const { return bIsProcessingFieldAttack; }
	FORCEINLINE const bool& CanInterruptFieldAttack() const { return bCanInterruptFieldAttack; }

#pragma endregion

#pragma region Weapon

protected:
	// Weapon Blueprint 받기 용도
	UPROPERTY(EditDefaultsOnly, Category = "Character Weapon")
	TSubclassOf<ARBWeaponTypeBase> WeaponClass;

	// C++ Weapon System 사용을 위한 용도
	UPROPERTY()
	TObjectPtr<ARBWeaponTypeBase> Weapon;

private:
	// BP로 설정한 무기를 캐릭터에 셋업
	void SetWeapon();

	// For Field Attack Interrupt and End Delegate
	virtual void IsFieldAttackHit() override;
	virtual void InterruptFieldAttack() override;
	void FieldAttackEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	bool bIsProcessingFieldAttack;
	bool bCanInterruptFieldAttack;

#pragma endregion

private:
	// 캐릭터 고유의 이름 (혹은 코드). 내부 구분용이므로 신경써서 작성
	UPROPERTY(EditAnywhere, Category = "Character Info", Meta = (AllowPrivateAccess = "true"))
	FName CharacterName = FName(NAME_None);
};
