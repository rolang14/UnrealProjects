#pragma once

#include "CoreMinimal.h"
#include "RBSelectionOption.generated.h"


USTRUCT(BlueprintType)
struct FRBSelectionOption
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FText Label;
};