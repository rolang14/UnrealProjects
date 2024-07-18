// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageType_FieldAttack.generated.h"

/**
 * 필드 공격에 대한 대미지 타입
 */
UCLASS(Const)
class PROJECTRB_API UDamageType_FieldAttack : public UDamageType
{
	GENERATED_BODY()
	
	// Can Add Icons and Force Feedback (Impulse) Data and so on...
};
