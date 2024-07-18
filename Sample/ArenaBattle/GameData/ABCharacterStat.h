
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FABCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
	   
	// Stat 구조체끼리 연산을 하는 경우 각각의 멤버를 더해주기 위해 float으로 고정시키고,
	// 아래와 같이 덧셈 연산자를 지정해줌으로써 구현함.
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		// 우선 FABCharacterStat을 float 포인터로 캐스팅하고
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		// 결과로 저장할 객체 역시
		FABCharacterStat Result;
		// float 포인터로 캐스팅한다.
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		// 그리고 float 멤버 개수만큼 (그렇기 때문에 모든 멤버를 float로 선언한 것이다)
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);
		// 돌아가며 각 인덱스끼리 합친다 (같은 종류끼리 더한다)
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		// 이렇게 하면 float 포인터(배열)에 의해 Result 값이 갱신되고
		// 이를 반환하면 덧셈이 마무리된다.
		return Result;
	}
};