// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Stat �� ������ �о ��Ī�Ǵ� �ؽ�Ʈ ��Ͽ� �ش� ������ �Է��ϵ��� ��.
	// StaticStruct �� ����ü ������ �����ͼ� Iterator ���� ���� ��,
	// ��� ����ü ���� ����� ��ȸ�Ѵ�.
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		// ��Ʈ�� �̸� ����� (BP�� �̸��� �̷� ���¿��� �Ѵ�.)
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		// TextBlock ��Ʈ�� ��������
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Emplace(PropKey, BaseTextBlock);
		}

		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());
		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Emplace(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// TFieldIterator : ���÷��ǿ� ���� ��ȸ ����ε�,
	// UCLASS Ȥ�� USTRUCT ��, �ش� ���ͷ����͸� ����ϸ� ��� �˻��� �����ϴ�.
	// ���⼭�� FABCharacterStat ����ü ������ ���ͼ� �ش� ����ü ���� Numeric Property ���� �����´�.
	// TFieldIterator �ڷ����� ���� ���� ���� �˰�����, Operator++ �� ���� ���ǵǾ��־�
	// �̷��� �ϸ� ���� �˻� ����� �Ѿ�� �Ǽ� ��� ����� ���� ��ȸ�� �����ϴ�.
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		// �̹� �˻� ����� ���� NumericProperty ��� ���� �̸��� �����´�.
		// �� ���, �츮�� float ������ ������ UPROPERTY ���÷��� ����� ����ϴ�
		// MaxHp ~ MovementSpeed ������ �������� �ȴ�.
		// �̰� ���� Key ������ GetName�� ���� ��������
		const FName PropKey(PropIt->GetName());
		UE_LOG(LogTemp, Log, TEXT("PropKey : %s"), *PropKey.ToString());

		float BaseData = 0.0f;
		// ���� ��� ���� PropIt�� MaxHp�� ����Ű�� �ִٸ�,
		// BaseStat �����̳� ������ MaxHp�����͸� BaseData�� ����
		// ���̵� �����ͷ� �޾Ƽ� �����ع�����.
		// ���θ� ���ϱ� Getter �� ������ Getter �� ģ���� �ҷ����ְ�
		// ���� ��� �ٷ� Memcpy ���ܹ�����.
		// FABCharacterStat ����ü�� Void �����ͷ� �޾Ƽ� �����Ѵ�. 
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);

		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		// ������ Ű�� ���� (MaxHp Ű�� �����) TextBlock ��Ʈ���� �����ͼ� (TMap �̹Ƿ� **)
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			// BaseData �� �̻ڰ� ���� �� FText �� ��ȯ�ؼ� �ش� Text Block�� SetText �Ѵ�.
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}