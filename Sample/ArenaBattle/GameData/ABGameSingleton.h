// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

// ���� �α� ī�װ��� LogABGameSingleton, Error �±׸� ����ؼ� ������.
DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UABGameSingleton();

	// Singleton Getter
	static UABGameSingleton& Get();
	
// Character Stat Data Section
public:
	// ĳ������ Level �� �ش��ϴ� ĳ���� ������ ���� ��.
	// �ռ� �𸮾� �����Ϳ��� �̱��� Ŭ������ csv ������ ���̺��� �����Ϸ��� �ݵ�� Name�� �־�� �ϸ� �̸� Key�� �����Ѵٰ� �ߴ�.
	// �׷��� �� Key�� ����Ͽ� �����ϴ� ��
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FABCharacterStat> CharacterStatTable;
};
