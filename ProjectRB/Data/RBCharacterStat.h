#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RBCharacterStat.generated.h"

/*
* 순수 캐릭터 스탯 종류
*/
USTRUCT(BlueprintType)
struct FRBCharacterStat : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FRBCharacterStat() : MaxHP(0.f), ATK(0.f), DEF(0.f), SPD(0.f), EXP(0.f) {}

	UPROPERTY(EditAnywhere, Category = "Character Stat")
	float MaxHP;

	// 캐릭터의 기초 공격력
	UPROPERTY(EditAnywhere, Category = "Character Stat")
	float ATK;

	// 캐릭터의 기초 방어력 (추후 공격 타입에 대한 방어력 혹은 물리/마법 정도라도 나누려고 생각중)
	UPROPERTY(EditAnywhere, Category = "Character Stat")
	float DEF;

	// 턴을 계산할 캐릭터의 기본 속도 (빠를수록 턴이 일찍 오도록 한다)
	UPROPERTY(EditAnywhere, Category = "Character Stat")
	float SPD;

	// 다음 레벨이 되기 위해 필요한 경험치
	UPROPERTY(EditAnywhere, Category = "Character Stat")
	float EXP;

	// 스탯에 대한 + 연산자 오버로딩
	FRBCharacterStat operator+(const FRBCharacterStat& Other) const
	{
		// 데이터를 float 배열로 받음
		const float* const ThisValue = reinterpret_cast<const float* const>(this);
		const float* const OtherValue = reinterpret_cast<const float* const>(&Other);

		// 결과를 저장할 구조체
		FRBCharacterStat Result;
		// 향후 문제가 되면 const 를 빼자.
		float* const ResultPtr = reinterpret_cast<float* const>(&Result);
		// float 멤버만 존재하므로
		int StatNum = sizeof(FRBCharacterStat) / sizeof(float);
		for (int8 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisValue[i] + OtherValue[i];
		}

		return Result;
	}
};