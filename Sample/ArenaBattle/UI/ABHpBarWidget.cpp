// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
// ProgressBar 등 UI 패키지를 사용하려면 UMG 모듈을 사용해야 하는데,
// 이게 기본적으로 포함이 안되어있어서 모듈에 추가를 해줘야 한다.
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 생성과 함께 MaxHp 초기화해야 올바르게 만들어짐.
	MaxHp = -1.0f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 위 단계가 끝나면 필요한 UI 및 기능들이 모두 초기화가 완료되었다고 봄.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	// 반드시 HpProgressBar 가 있어야 하기 때문에 Ensure.
	ensure(HpProgressBar);

	// HP TEXT 추가
	HpTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpTextBlock);

	// 이제 얻어낸 액터 정보에 델리게이트를 등록해서 해당 액터의 HpBar가 변하도록 한다.
	// 다만, 직접적으로 ABCharacterBase 의 클래스 정보를 참조하면 의존성이 발생하기 때문에
	// 다양한 캐릭터에 적용이 가능하도록 설계를 해야 한다. -> 인터페이스를 사욯
	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}

// 스탯이 업데이트 될 때 (스탯 변경에 의해)
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

// HpBar 가 업데이트 될 때 (Current Hp 변경에 의해)
void UABHpBarWidget::UpdateHpBar(const float NewCurrentHp)
{
	// CurrentHp 를 반영하도록 수정
	CurrentHp = NewCurrentHp;

	// MaxHp가 기준 수치 이상인지 확실하게 검사.
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
