#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RBInteractDialogueData.generated.h"


USTRUCT(BlueprintType)
struct FRBInteractDialogueData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FText Speaker;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FText Script;

	// USoundBase Asset Name
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSoftObjectPtr<USoundBase> Voice;
};