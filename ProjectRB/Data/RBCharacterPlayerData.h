#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Character/RBCharacterPlayer.h"
#include "RBCharacterPlayerData.generated.h"

/*
* 
*/
USTRUCT(BlueprintType)
struct FRBCharacterPlayerData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Character Name")
	FName CharacterName;

	UPROPERTY(EditAnywhere, Category = "Character BP")
	TSubclassOf<ARBCharacterPlayer> CharacterBP;
};