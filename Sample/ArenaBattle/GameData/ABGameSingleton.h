// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

// 현재 로그 카테고리를 LogABGameSingleton, Error 태그를 사용해서 지정함.
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
	// 캐릭터의 Level 에 해당하는 캐릭터 스탯을 가져 옴.
	// 앞서 언리얼 에디터에서 싱글톤 클래스로 csv 데이터 테이블을 관리하려면 반드시 Name이 있어야 하며 이를 Key로 관리한다고 했다.
	// 그래서 이 Key를 사용하여 구현하는 것
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FABCharacterStat> CharacterStatTable;
};
