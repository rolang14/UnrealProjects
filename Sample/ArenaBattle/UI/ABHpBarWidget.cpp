// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
// ProgressBar �� UI ��Ű���� ����Ϸ��� UMG ����� ����ؾ� �ϴµ�,
// �̰� �⺻������ ������ �ȵǾ��־ ��⿡ �߰��� ����� �Ѵ�.
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// ������ �Բ� MaxHp �ʱ�ȭ�ؾ� �ùٸ��� �������.
	MaxHp = -1.0f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// �� �ܰ谡 ������ �ʿ��� UI �� ��ɵ��� ��� �ʱ�ȭ�� �Ϸ�Ǿ��ٰ� ��.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	// �ݵ�� HpProgressBar �� �־�� �ϱ� ������ Ensure.
	ensure(HpProgressBar);

	// HP TEXT �߰�
	HpTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpTextBlock);

	// ���� �� ���� ������ ��������Ʈ�� ����ؼ� �ش� ������ HpBar�� ���ϵ��� �Ѵ�.
	// �ٸ�, ���������� ABCharacterBase �� Ŭ���� ������ �����ϸ� �������� �߻��ϱ� ������
	// �پ��� ĳ���Ϳ� ������ �����ϵ��� ���踦 �ؾ� �Ѵ�. -> �������̽��� ��G
	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}

// ������ ������Ʈ �� �� (���� ���濡 ����)
void UABHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpTextBlock)
	{
		HpTextBlock->SetText(FText::FromString(GetHpStatText()));
	}
}

// HpBar �� ������Ʈ �� �� (Current Hp ���濡 ����)
void UABHpBarWidget::UpdateHpBar(const float NewCurrentHp)
{
	// CurrentHp �� �ݿ��ϵ��� ����
	CurrentHp = NewCurrentHp;

	// MaxHp�� ���� ��ġ �̻����� Ȯ���ϰ� �˻�.
	ensure(MaxHp > 0);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}

	if (HpTextBlock)
	{
		HpTextBlock->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UABHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp);
}
