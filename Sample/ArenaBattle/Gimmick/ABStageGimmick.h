// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

// ���¿� ���� ��������Ʈ ���� (switch ���)
DECLARE_DELEGATE(FOnStageChangedDelegate);
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()

	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InStageChangedDelegate) : StageChangedDelegate(InStageChangedDelegate) {}

	FOnStageChangedDelegate StageChangedDelegate;
};

// �������� ���� �Ǵ��� ���� ������
UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABStageGimmick();

// Stage Stat
public:
	FORCEINLINE int32 GetStageNum() const { return CurrentStageNum; }
	FORCEINLINE void SetStageNum(int32 InNewStageNum) { CurrentStageNum = InNewStageNum; }

	// �����Ϳ��� State ���濡 ���� ���ε� �Լ��� �ùķ��̼� �ǵ��� �ϱ� ����.
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Stage Section
protected:
	// �������� �޽�
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;

	// �������� Ʈ����
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	// �������� �޾��� �Լ�
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// For Gate Section
protected:
	// ����Ʈ�� 4���� �����Ƿ� �̸� ���� �迭�� ������(���� �̸����� Mesh�� ��������) �����Ϸ��� ��.
	// ����Ʈ �̸��� �޽�
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	// ����Ʈ Ʈ���� �迭
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	// ���������� ����Ʈ ������ �Լ�
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// �� ���� �ݱ�
	void OpenAllGates();
	void CloseAllGates();

	// State Section
protected:
	// ���� �������� ����
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	// ������ SetState �Լ� ȣ���� ���ؼ� ���� ������ �̷�������� ��.
	void SetState(const EStageState InNewState);

	// Delegate
	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StageChangedActions;

	// �� ���º� ��������Ʈ ���ε� �Լ�
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

	// Fight Section
protected:
	// ������ �����ų NPC
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	// TSubclassOf �� �츮�� ���ø����� ������ Ŭ������ ���� Ŭ�����鸸�� ������ �� �ֵ��� �� �ش�.
	// �׷��Ƿ� ���� �츮�� ������ NPC�� �ݵ�� AABCharacterNonPlayer Ŭ���� ����� ���� �����ϰ�
	// �� NPC�� ����� ����� �Ѵٸ� �̷��� ����� �� �ִ� ���̴�.
	// �̸� ���������ڶ�� �Ѵ�.
	TSubclassOf<class AABCharacterNonPlayer> OpponentClass;

	// �� ���� ������
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	// ���� ����ϴ� ��� ����ܰ�� �����ϴ� �Լ�
	UFUNCTION()
	void OnOpponentDestroy(AActor* DestroyedActor);

	// �� ���� �����̸� ���� Ÿ�̸� ����
	FTimerHandle OpponentTimerHandle;
	// ���� ��ȯ�� �� Destory�� ���ε��ϱ� ����
	void OnOpponentSpawn();

	// Reward Section
protected:
	// ���� ���� ������ ���� Ŭ���� ���� ����
	// �ٵ� ��� �� Visible�ϱ�? ��ü Ÿ���� Visible�� ���� �Ѵٴµ� �׷� �� �������� Edit�ϱ� ��
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> RewardBoxClass;

	// ItemBox�� ������ �����ϱ� TArray�� �����Ϸ��� ����ε�, �� TObjectPtr�� �ƴ� TWeakObjectPtr�� ����ұ�?
	// ��ǻ� �� �����۹ڽ��� ��Ϳ��� ������ �����Ѱ� �ƴ϶�, �ڱ� ������ ������ ���� ���⼭�� ��ġ�� ���� ���̴�.
	// ������ ������ �Ҹ�� �� �ִµ�, �̸� TObjectPtr (������) �� �����ϰ� �Ǹ�
	// �𸮾� ������ �̰� �Ҹ�Ǵ��� Gimmick������ �� ��ü �޸𸮴� �Ҹ��Ű�� �ʴ´�!
	// TWeakObjectPtr�� �����ϸ� �� �ڽ��� �Ҹ��ϴ� ��� Gimmick������ ���� �޸𸮿��� ������Ű�Ƿ�
	// �������� ������ �ؾ� �Ѵ�.
	// 
	// �Ѹ���� ����� ���� ������ �� ��ü�� �Ҹ�� �� �Ҹ��ؾ� �ϴ� ��쿡�� �������� �������.
	// �׷��� ��¥ ����� ����, ���� ��� ĳ���Ϳ� �Ӹ�ī���� �и��Ǿ� ���� ���
	// �Ӹ�ī���� ���� �پ� Ÿ�� ���������� ���߿� �ٽ� �Ӹ�ī���� �ڶ�� ���
	// ĳ���Ͱ� �Ҹ�� �� �Ӹ�ī���� ���� �Ҹ�Ǵ°� �����Ƿ� �̴� �������� �ؾ� ������
	// 
	// �� ���������� ���� ����ó�� ���� ���� ������ ���� �������ڰ� �Ҹ�� ��
	// �׷����� �ұ��ϰ� ���������� ����ؼ� ���� ���ڸ� �����ؾ� �Ǵ� ��Ȳ�� �ƴ϶��,
	// ���� ������ �Ҹ�� ���������� ���谡 �����Ƿ� �̷� ��쿡�� �������� �����ϴ°��� �´�.
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> RewardBoxes;

	// ���� ���� ��ġ�� �̸����� �����ϱ� ����
	TMap<FName, FVector> RewardBoxLocations;

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnBoxes();

// Stage Stat
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum;
};
