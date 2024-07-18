// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RBSelectionOption.h"
#include "RBSelectionBase.generated.h"


class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTRB_API URBSelectionBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	URBSelectionBase();
	virtual void NativeConstruct() override;

	void Clear();
	void AddOption(const FRBSelectionOption& InOption);
	void SetCurrentSelection(int InIndex);

	// 앞, 뒤 선택
	UFUNCTION(BlueprintCallable)
	void SelectPrevious();

	UFUNCTION(BlueprintCallable)
	void SelectNext();

	DECLARE_DELEGATE_OneParam(FOnSelectionChanged, int)
	FOnSelectionChanged OnSelectionChanged;

protected:
	// For Keyboard or Controller Navigation
	UFUNCTION()
	UWidget* OnNavigation(EUINavigation InNavigation);

	// Label을 현재 선택된 옵션으로 변경
	void UpdateCurrentSelection();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRBSelectionOption> Options;

	// 위젯 바인드 "Label"
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> Label;
	
	int CurrentSelection;
};
