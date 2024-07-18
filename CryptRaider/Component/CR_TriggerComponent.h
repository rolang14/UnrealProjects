// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CR_OverlapBoxComponentBase.h"
#include "CR_TriggerComponent.generated.h"

/**
 *
 */

// BlueprintSpawnableComponent 키워드가 없으면 블루프린트에서 해당 컴포넌트를 검색할 수가 없다.
// ClassGroup 는 말 그대로 이 클래스가 들어갈 카테고리를 의미함. (BP에서 검색할떄 나온다)
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UCR_TriggerComponent : public UCR_OverlapBoxComponentBase
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetChangeComponent(UActorComponent* NewChangeComponent);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Action Target")
	FName UnlockTag;

	void SetMeshChange(AActor* InOverlappingActor) const;
	void SetMoverMove(const bool IsReadyToMove) const;

	UActorComponent* ChangeComponent;
};
