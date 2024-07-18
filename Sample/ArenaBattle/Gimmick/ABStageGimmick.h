// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

// 상태에 따른 델리게이트 구현 (switch 대용)
DECLARE_DELEGATE(FOnStageChangedDelegate);
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()

	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InStageChangedDelegate) : StageChangedDelegate(InStageChangedDelegate) {}

	FOnStageChangedDelegate StageChangedDelegate;
};

// 스테이지 상태 판단을 위한 열거형
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

	// 에디터에서 State 변경에 따라 매핑된 함수가 시뮬레이션 되도록 하기 위함.
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Stage Section
protected:
	// 스테이지 메시
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;

	// 스테이지 트리거
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	// 오버랩을 받아줄 함수
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// For Gate Section
protected:
	// 게이트는 4개가 있으므로 이를 각각 배열과 맵으로(방향 이름으로 Mesh가 나오도록) 관리하려고 함.
	// 게이트 이름과 메시
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	// 게이트 트리거 배열
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	// 마찬가지로 게이트 오버랩 함수
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 문 열고 닫기
	void OpenAllGates();
	void CloseAllGates();

	// State Section
protected:
	// 현재 스테이지 상태
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	// 언제나 SetState 함수 호출을 통해서 상태 변경이 이루어지도록 함.
	void SetState(const EStageState InNewState);

	// Delegate
	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StageChangedActions;

	// 각 상태별 델리게이트 바인딩 함수
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

	// Fight Section
protected:
	// 적으로 등장시킬 NPC
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	// TSubclassOf 은 우리가 탬플릿으로 선언한 클래스의 하위 클래스들만을 선택할 수 있도록 해 준다.
	// 그러므로 만약 우리가 앞으로 NPC를 반드시 AABCharacterNonPlayer 클래스 상속을 통해 생성하고
	// 이 NPC를 멤버로 잡고자 한다면 이렇게 사용할 수 있는 것이다.
	// 이를 한정지정자라고 한다.
	TSubclassOf<class AABCharacterNonPlayer> OpponentClass;

	// 적 스폰 딜레이
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	// 적이 사망하는 경우 보상단계로 변경하는 함수
	UFUNCTION()
	void OnOpponentDestroy(AActor* DestroyedActor);

	// 적 스폰 딜레이를 위한 타이머 지정
	FTimerHandle OpponentTimerHandle;
	// 적이 소환될 때 Destory를 바인딩하기 위함
	void OnOpponentSpawn();

	// Reward Section
protected:
	// 보상 상자 스폰을 위한 클래스 한정 지정
	// 근데 얘는 왜 Visible일까? 객체 타입은 Visible로 보통 한다는데 그럼 왜 위에서는 Edit일까 흠
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> RewardBoxClass;

	// ItemBox를 여러개 놓으니깐 TArray로 관리하려는 모양인데, 왜 TObjectPtr이 아닌 TWeakObjectPtr을 사용할까?
	// 사실상 이 아이템박스는 기믹에서 로직을 설정한게 아니라, 자기 스스로 로직을 갖고 여기서는 배치만 해줄 뿐이다.
	// 때문에 스스로 소멸될 수 있는데, 이를 TObjectPtr (강참조) 로 선언하게 되면
	// 언리얼 엔진은 이게 소멸되더라도 Gimmick에서의 이 객체 메모리는 소멸시키지 않는다!
	// TWeakObjectPtr로 선언하면 이 박스가 소멸하는 경우 Gimmick에서도 역시 메모리에서 삭제시키므로
	// 약참조로 선언을 해야 한다.
	// 
	// 한마디로 멤버로 쓰긴 하지만 이 객체가 소멸될 때 소멸해야 하는 경우에는 약참조를 사용하자.
	// 그러나 진짜 멤버와 같이, 예를 들면 캐릭터와 머리카락이 분리되어 있을 경우
	// 머리카락이 불이 붙어 타서 없어지더라도 나중에 다시 머리카락이 자라는 경우
	// 캐릭터가 소멸될 때 머리카락이 같이 소멸되는게 맞으므로 이는 강참조를 해야 하지만
	// 
	// 이 스테이지와 보상 상자처럼 서로 정말 연관이 없고 보물상자가 소멸될 때
	// 그럼에도 불구하고 스테이지가 계속해서 보상 상자를 참조해야 되는 상황이 아니라면,
	// 보상 상자의 소멸과 스테이지는 관계가 없으므로 이런 경우에는 약참조로 관리하는것이 맞다.
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> RewardBoxes;

	// 보상 상자 위치를 이름으로 관리하기 위함
	TMap<FName, FVector> RewardBoxLocations;

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnBoxes();

// Stage Stat
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum;
};
