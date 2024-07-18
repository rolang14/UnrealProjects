// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RBWeaponTypeBase.h"

#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Sets default values
ARBWeaponTypeBase::ARBWeaponTypeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	// 나이아가라 컴포넌트 SET UP
	WeaponSummonEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Summon Effect"));
	WeaponSummonEffect->SetupAttachment(WeaponMesh);

	// MPC 불러오기
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> SummonEffectParameterRef(TEXT("/Game/ProjectRB/Weapon/Pipe/Material/MPC_Pipe_Edge.MPC_Pipe_Edge"));
	if (SummonEffectParameterRef.Object)
	{
		SummonEffectParameter = SummonEffectParameterRef.Object;
	}
}

void ARBWeaponTypeBase::BeginPlay()
{
	Super::BeginPlay();

	if (SummonEffectParameter)
	{
		SummonEffectPci = GetWorld()->GetParameterCollectionInstance(SummonEffectParameter);

		if (SummonEffectPci)
		{
			SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), 0.f);
		}

		if (WeaponSummonEffect)
		{
			WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.ParticleMask"), 1.f);
			WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.MeshMask"), 1.f);
		}
	}
}

void ARBWeaponTypeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Mask Smoothing 을 위해 Tick에서 Ease Out
	// 나중에 더 좋은 방법이 있다면 변경..
	if (bIsSummoning && SummonMaskValue <= 0.99f)
	{
		SummonMaskValue = FMath::InterpEaseOut<float>(SummonMaskValue, 1.0f, DeltaSeconds / WeaponSummonSpeed, 90.f);
		SetWeaponOnOffVFX(SummonMaskValue);
	}
	else if (!bIsSummoning && SummonMaskValue >= 0.01f)
	{
		//SummonMaskValue = FMath::FInterpTo(SummonMaskValue, 0.f, DeltaSeconds, WeaponDisappearSpeed);
		//SummonMaskValue = FMath::InterpEaseIn<float>(SummonMaskValue, 0.0f, DeltaSeconds / WeaponDisappearSpeed, 90.f);
		SummonMaskValue = FMath::InterpEaseOut<float>(SummonMaskValue, 0.0f, DeltaSeconds / WeaponDisappearSpeed, 90.f);
		SetWeaponOnOffVFX(SummonMaskValue);
	}
}

void ARBWeaponTypeBase::PerformWeaponFieldAttack()
{

}

void ARBWeaponTypeBase::PerformWeaponSummonEffect()
{
	bIsSummoning = true;
	WeaponSummonEffect->Activate();
}

void ARBWeaponTypeBase::PerformWeaponDisappearEffect()
{
	bIsSummoning = false;
	WeaponSummonEffect->Activate();
}

// Summon 이펙트 효과 Mask Value 를 변경
void ARBWeaponTypeBase::SetWeaponOnOffVFX(const float InMaskValue) const
{
	if (InMaskValue >= 0.99f || InMaskValue <= 0.01f)
	{
		UE_LOG(LogTemp, Display, TEXT("Niagara Deactivated"));
		WeaponSummonEffect->Deactivate();
	}

	if (SummonEffectPci)
	{
		SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), SummonMaskValue);
	}

	if (WeaponSummonEffect)
	{
		WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.MeshMask"), SummonMaskValue);
		WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.ParticleMask"), 1 - SummonMaskValue);
	}
}