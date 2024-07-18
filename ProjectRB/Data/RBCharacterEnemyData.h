#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Character/RBCharacterEnemy.h"
#include "RBCharacterEnemyData.generated.h"

/*
* 
*/
USTRUCT(BlueprintType)
struct FRBCharacterEnemyData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Character Name")
	FName CharacterName;

	UPROPERTY(EditAnywhere, Category = "Character BP")
	TSubclassOf<ARBCharacterEnemy> CharacterBP;
};