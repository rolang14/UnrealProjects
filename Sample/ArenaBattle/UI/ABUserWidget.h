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
	// 뭘 따로 생성자에다가 구현할게 아니기 때문에 굳이 가져와서 쓸 필요는 없다.
	//UABUserWidget(const FObjectInitializer& ObjectInitializer);
	// Getter, Setter 용도
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }
	FORCEINLINE const AActor* GetOwningActor() const { return OwningActor; }

protected:
	// 현재 이 위젯을 소유하고 있는 액터 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor ;
};
