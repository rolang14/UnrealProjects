// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RBCharacterBase.h"
#include "RBCharacterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRB_API ARBCharacterEnemy : public ARBCharacterBase
{
	GENERATED_BODY()
	
public:
	//FORCEINLINE TArray<TSubclassOf<ARBCharacterEnemy>> GetEnemyParty() const { return EnemyParty; }
	FORCEINLINE TSoftObjectPtr<UDataTable> GetEnemyPartyInfo() const { return EnemyPartyInfo; }
	
private:
	// 배틀 인카운터 멤버
	// 레벨이 필요하므로 FStruct 를 통해 인카운터 멤버 구조체를 정의하고
	// 이를 참조하도록 하자.. BP는 GameInstance 가 갖고 있으니 매칭만 하면 된다.
	// 예를 들면 FName, float Lv 를 갖는 구조체를 정의하고
	// GameInstance 에서는 이를 읽어서 FName에 매칭되는 BP정보를 전투 레벨에 알려주고
	// 다시 Lv를 통해 어떤 스탯을 부여하면 되는지 매칭할 수 있도록 한다. (이것은 또 역시 데이터 테이블을 사용하여 해당 몬스터의 스펙을 Lv별로 정리하면 된다.)
	// 이걸 하기 위해서는 스탯을 정의해야 한다.
	// 스탯 시스템은 Gameplay Ability System 을 보면 알 수 있으니까 이거 강의를 보자. 유튜브에 저번에 있더라.
	//UPROPERTY(EditInstanceOnly, Category = "Battle Info")
	//TArray<TSubclassOf<ARBCharacterEnemy>> EnemyParty;

	UPROPERTY(EditInstanceOnly, Category = "Battle Info")
	TSoftObjectPtr<UDataTable> EnemyPartyInfo;
};
