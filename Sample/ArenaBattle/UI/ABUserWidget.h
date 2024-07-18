// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// �� ���� �����ڿ��ٰ� �����Ұ� �ƴϱ� ������ ���� �����ͼ� �� �ʿ�� ����.
	//UABUserWidget(const FObjectInitializer& ObjectInitializer);
	// Getter, Setter �뵵
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }
	FORCEINLINE const AActor* GetOwningActor() const { return OwningActor; }

protected:
	// ���� �� ������ �����ϰ� �ִ� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor ;
};
