// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		// Array 를 생성하여 캐릭터 스탯 데이터 테이블을 Get
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	// 별도의 레벨 데이터를 두지 않고 Table의 Row을 캐릭터 레벨로 취급한다.
	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton* UABGameSingleton::Get()
{
	// 싱글톤은 반드시 엔진 초기화 시점에 인스턴스가 생성되어야 하므로 강한 체크
	// 싱글톤은 GEngine 에서 관리하도록 한다.
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	return IsValid(Singleton) ?
		Singleton : Singleton = NewObject<UABGameSingleton>();
}
