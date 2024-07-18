// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Interface/ABCharacterStateInterface.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterBase.generated.h"

// 로그를 찍기 위함
// LogABCharacter 라는 카테고리로 로그 카테고리를 지정한것임.
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

// 컨트롤 타입 변경을 위한 구조체 선언
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter
};

// 아이템의 타입에 따라 다른 행동을 하도록 구현
// 근데 우리는 여기서 이를 다수의 배열로 관리하고싶은데, 배열로 만들 수가 없다.
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);
// 따라서 이를 감싸는 래퍼 델리게이트 구조체를 하나 만든다.
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	
	// 생성자
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}

	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface, public IABCharacterWidgetInterface, public IABCharacterItemInterface, public IABCharacterStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	// Begin Play가 실행되기 전 : PostInitializeComponents 에다가 죽는 모션을 추가함
	// 머 딱히 이 시점이 아니더라도 Begin Play에서 해도 상관 없는데 그냥 한번 써보는 의미로 해봄
	virtual void PostInitializeComponents() override;

protected:
	// Control Data 를 받아서 설정하기 위한 함수 지정
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	// 각각 컨트롤 타입, 컨트롤 세팅값을 받아올 것이므로 맵으로 선언
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

// For Combo Action Section
protected:
	// 몽타주용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	// 블루프린트에서 안쓰고 그냥 내부에서 사용할 Combo 값이므로 따로 UPROPERTY를 사용하진 않음.
	// 현재 콤보 상태. 0 = 콤보를 시작하지 않음. 1~4 = 콤보 단계
	int32 CurrentCombo = 0;

	void ProcessComboCommand();

	// 콤보 시작, 콤보 종료에 대한 함수를 생성할건데
	void ComboActionBegin();
	// ComboActionEnd의 경우에는 몽타주에 설정된 Delegate 를 통해서 바로 호출될 수 있도록 파라미터를 맞춘다.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// AI용 콤보 끝나는 시점
	virtual void NotifyComboActionEnd();

	// 타이머를 이용해 원하는 타이밍에 특정 함수를 호출하도록 설정할 수 있다.
	FTimerHandle ComboTimerHandle;
	// 특정 타이밍 이전에 콤보 커맨드가 들어왔는지를 확인
	// UPROPERTY 안쓸거고 그냥 로직을 위한거니까 uint8b 이렇게 선언 안해도 된다.
	bool HasNextComboCommand = false;

	// 몽타주마다 특정 프레임에서 타이머를 발생시키기 위한 함수
	void SetComboCheckTimer();
	// 타이머 발동 시 입력이 들어왔는지 안들어왔는지를 체크
	void ComboCheck();

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	// 대미지 계산을 위해 언리얼에서 제공되는 함수 오버라이드 (AActor에 정의됨)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCause) override;

// For Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadActionMontage;

	// 사망 설정
	virtual void SetDead();
	// 사망 모션 재생용
	void PlayDeadAnimation();

	// 죽어서 사라지기까지의 시간
	float DeadEventDelayTime = 5.0f;

// For Character Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

// For UI Widget Section
protected:
	// UABHpBarWidget 클래스를 바로 사용하는것이 아니라, 컴포넌트를 이용해서 붙여야 하므로 컴포넌트 포인터를 선언.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;

	// 캐릭터 위젯 추가 인터페이스
	virtual void SetUpCharacterWidget(class UABUserWidget* InUserWidget) override;

// Item Section
protected:
	// 무기 장착용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// 각각의 아이템에 대한 액션 정의용 Delegate 래퍼 배열
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;
	
	virtual void TakeItem(class UABItemData* InItemData) override;
	
	// 각각 포션, 무기, 스크롤에 대한 액션
	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

// Stat Section
public:
	// 캐릭터 레벨 Getter
	const int32 GetLevel() const;
	// 캐릭터 레벨 Setter
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

// For Character State
protected:
	FORCEINLINE const bool GetAliveState() const { return bIsAlive; }
	FORCEINLINE void SetAliveState(const bool InAliveState) { bIsAlive = InAliveState; }

	virtual const bool CharacterAliveCheck() const override;

	bool bIsAlive;
};
