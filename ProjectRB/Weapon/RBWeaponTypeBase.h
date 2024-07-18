// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/RBWeaponFieldAttackInterface.h"
#include "RBWeaponTypeBase.generated.h"

// 전방선언
class UNiagaraComponent;
class UMaterialParameterCollection;

// 무기 타입 열거 클래스
UENUM()
enum class WEAPON_TYPE : uint8
{
	NONE = 0,
	OHSWORD,
	THSWORD,
	GUN,
	MAGIC
};

/**
 * 무기 타입에 대한 기초 클래스. 모든 무기는 이 클래스의 하위 클래스를 상속받아야 한다.
 */
UCLASS(Abstract)
class PROJECTRB_API ARBWeaponTypeBase : public AActor, public IRBWeaponFieldAttackInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARBWeaponTypeBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Weapon Mesh")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	// 무기 자동 사라짐/소환 이펙트를 위한 나이아가라 컴포넌트
	UPROPERTY(VisibleInstanceOnly, Category = "Weapon Effect")
	TObjectPtr<UNiagaraComponent> WeaponSummonEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Effect")
	TSoftObjectPtr<UParticleSystem> WeaponAttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Effect")
	TSoftObjectPtr<USoundBase> WeaponAttackSound;

	// 해당 무기가 적중했을 때의 피격 이펙트. 상대 캐릭터가 아닌 무기의 멤버로 존재하는것이 낫다고 판단함.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Effect")
	TSoftObjectPtr<UParticleSystem> WeaponHitEffect;

	// 이유는 잘 모르겠지만.. MPC는 블루프린트 변수로 나타나질 않고, 월드로부터 직접 참조를 해야 한다고 한다.
	// 때문에 Constructor에서 직접적으로 오브젝트 참조를 해야 할 듯 하다.
	UPROPERTY(EditAnywhere, Category = "Weapon Effect")
	TObjectPtr<UMaterialParameterCollection> SummonEffectParameter;

public:
	// 무기의 타입
	UPROPERTY(VisibleAnywhere, Category = "Weapon Type")
	WEAPON_TYPE WeaponType = WEAPON_TYPE::NONE;

	// 무기 소환 이펙트 속도
	UPROPERTY(EditAnywhere, Category = "Weapon Type")
	float WeaponSummonSpeed = 1.f;

	// 무기 소멸 이펙트 속도
	UPROPERTY(EditAnywhere, Category = "Weapon Type")
	float WeaponDisappearSpeed = 1.f;
	
	// 각각의 무기에 맞는 공격 방식 수행
	// 총 : 발사체 발사
	// 검 : 흠... 뭐가 필요할까 딱히 없다면 이건 없애고 그냥 발사체류만 따로 묶어서 정의하는 것으로 하자
	virtual void PerformWeaponFieldAttack() override;
	// 필드 공격을 수행할 때 무기 소환 이펙트 재생
	virtual void PerformWeaponSummonEffect() override;
	// 필드 공격을 수행 후 무기 소멸 이펙트 재생
	virtual void PerformWeaponDisappearEffect() override;

private:
	void SetWeaponOnOffVFX(const float InMaskValue) const;

	UPROPERTY()
	TObjectPtr<UMaterialParameterCollectionInstance> SummonEffectPci;

	float SummonMaskValue = 0.f;
	bool bIsSummoning = false;
};
