
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
	   
	// Stat ����ü���� ������ �ϴ� ��� ������ ����� �����ֱ� ���� float���� ������Ű��,
	// �Ʒ��� ���� ���� �����ڸ� �����������ν� ������.
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		// �켱 FABCharacterStat�� float �����ͷ� ĳ�����ϰ�
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		// ����� ������ ��ü ����
		FABCharacterStat Result;
		// float �����ͷ� ĳ�����Ѵ�.
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		// �׸��� float ��� ������ŭ (�׷��� ������ ��� ����� float�� ������ ���̴�)
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);
		// ���ư��� �� �ε������� ��ģ�� (���� �������� ���Ѵ�)
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		// �̷��� �ϸ� float ������(�迭)�� ���� Result ���� ���ŵǰ�
		// �̸� ��ȯ�ϸ� ������ �������ȴ�.
		return Result;
	}
};