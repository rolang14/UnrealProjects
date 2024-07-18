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

// �α׸� ��� ����
// LogABCharacter ��� ī�װ��� �α� ī�װ��� �����Ѱ���.
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

// ��Ʈ�� Ÿ�� ������ ���� ����ü ����
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter
};

// �������� Ÿ�Կ� ���� �ٸ� �ൿ�� �ϵ��� ����
// �ٵ� �츮�� ���⼭ �̸� �ټ��� �迭�� �����ϰ������, �迭�� ���� ���� ����.
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);
// ���� �̸� ���δ� ���� ��������Ʈ ����ü�� �ϳ� �����.
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	
	// ������
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

	// Begin Play�� ����Ǳ� �� : PostInitializeComponents ���ٰ� �״� ����� �߰���
	// �� ���� �� ������ �ƴϴ��� Begin Play���� �ص� ��� ���µ� �׳� �ѹ� �Ẹ�� �ǹ̷� �غ�
	virtual void PostInitializeComponents() override;

protected:
	// Control Data �� �޾Ƽ� �����ϱ� ���� �Լ� ����
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	// ���� ��Ʈ�� Ÿ��, ��Ʈ�� ���ð��� �޾ƿ� ���̹Ƿ� ������ ����
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

// For Combo Action Section
protected:
	// ��Ÿ�ֿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	// �������Ʈ���� �Ⱦ��� �׳� ���ο��� ����� Combo ���̹Ƿ� ���� UPROPERTY�� ������� ����.
	// ���� �޺� ����. 0 = �޺��� �������� ����. 1~4 = �޺� �ܰ�
	int32 CurrentCombo = 0;

	void ProcessComboCommand();

	// �޺� ����, �޺� ���ῡ ���� �Լ��� �����Ұǵ�
	void ComboActionBegin();
	// ComboActionEnd�� ��쿡�� ��Ÿ�ֿ� ������ Delegate �� ���ؼ� �ٷ� ȣ��� �� �ֵ��� �Ķ���͸� �����.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// AI�� �޺� ������ ����
	virtual void NotifyComboActionEnd();

	// Ÿ�̸Ӹ� �̿��� ���ϴ� Ÿ�ֿ̹� Ư�� �Լ��� ȣ���ϵ��� ������ �� �ִ�.
	FTimerHandle ComboTimerHandle;
	// Ư�� Ÿ�̹� ������ �޺� Ŀ�ǵ尡 ���Դ����� Ȯ��
	// UPROPERTY �Ⱦ��Ű� �׳� ������ ���ѰŴϱ� uint8b �̷��� ���� ���ص� �ȴ�.
	bool HasNextComboCommand = false;

	// ��Ÿ�ָ��� Ư�� �����ӿ��� Ÿ�̸Ӹ� �߻���Ű�� ���� �Լ�
	void SetComboCheckTimer();
	// Ÿ�̸� �ߵ� �� �Է��� ���Դ��� �ȵ��Դ����� üũ
	void ComboCheck();

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	// ����� ����� ���� �𸮾󿡼� �����Ǵ� �Լ� �������̵� (AActor�� ���ǵ�)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCause) override;

// For Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadActionMontage;

	// ��� ����
	virtual void SetDead();
	// ��� ��� �����
	void PlayDeadAnimation();

	// �׾ ������������ �ð�
	float DeadEventDelayTime = 5.0f;

// For Character Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

// For UI Widget Section
protected:
	// UABHpBarWidget Ŭ������ �ٷ� ����ϴ°��� �ƴ϶�, ������Ʈ�� �̿��ؼ� �ٿ��� �ϹǷ� ������Ʈ �����͸� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;

	// ĳ���� ���� �߰� �������̽�
	virtual void SetUpCharacterWidget(class UABUserWidget* InUserWidget) override;

// Item Section
protected:
	// ���� ������ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// ������ �����ۿ� ���� �׼� ���ǿ� Delegate ���� �迭
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;
	
	virtual void TakeItem(class UABItemData* InItemData) override;
	
	// ���� ����, ����, ��ũ�ѿ� ���� �׼�
	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

// Stat Section
public:
	// ĳ���� ���� Getter
	const int32 GetLevel() const;
	// ĳ���� ���� Setter
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

// For Character State
protected:
	FORCEINLINE const bool GetAliveState() const { return bIsAlive; }
	FORCEINLINE void SetAliveState(const bool InAliveState) { bIsAlive = InAliveState; }

	virtual const bool CharacterAliveCheck() const override;

	bool bIsAlive;
};
