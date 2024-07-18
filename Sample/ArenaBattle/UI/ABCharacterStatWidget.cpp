// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Stat 의 정보를 읽어서 매칭되는 텍스트 블록에 해당 정보를 입력하도록 함.
	// StaticStruct 로 구조체 정보를 가져와서 Iterator 으로 받은 뒤,
	// 모든 구조체 내부 멤버를 순회한다.
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		// 컨트롤 이름 만들기 (BP에 이름이 이런 형태여야 한다.)
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		// TextBlock 컨트롤 가져오기
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
	// TFieldIterator : 리플렉션에 대한 순회 기능인데,
	// UCLASS 혹은 USTRUCT 등, 해당 이터레이터를 사용하면 모두 검색이 가능하다.
	// 여기서는 FABCharacterStat 구조체 정보를 얻어와서 해당 구조체 내의 Numeric Property 들을 가져온다.
	// TFieldIterator 자료형에 직접 가서 보면 알겠지만, Operator++ 이 따로 정의되어있어
	// 이렇게 하면 다음 검색 결과로 넘어가게 되서 모든 결과에 대한 순회가 가능하다.
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		// 이번 검색 결과에 대한 NumericProperty 멤버 변수 이름을 가져온다.
		// 이 경우, 우리가 float 형으로 선언한 UPROPERTY 리플렉션 기능을 사용하는
		// MaxHp ~ MovementSpeed 값들을 가져오게 된다.
		// 이게 이제 Key 값으로 GetName에 의해 가져와짐
		const FName PropKey(PropIt->GetName());
		UE_LOG(LogTemp, Log, TEXT("PropKey : %s"), *PropKey.ToString());

		float BaseData = 0.0f;
		// 예를 들어 현재 PropIt이 MaxHp를 가리키고 있다면,
		// BaseStat 컨테이너 내부의 MaxHp데이터를 BaseData로 복사
		// 보이드 포인터로 받아서 복사해버린다.
		// 내부를 보니까 Getter 이 있으면 Getter 로 친절히 불러와주고
		// 없는 경우 바로 Memcpy 갈겨버린다.
		// FABCharacterStat 구조체를 Void 포인터로 받아서 복사한다. 
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);

		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		// 동일한 키를 가진 (MaxHp 키에 저장된) TextBlock 컨트롤을 가져와서 (TMap 이므로 **)
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			// BaseData 를 이쁘게 만든 후 FText 로 변환해서 해당 Text Block에 SetText 한다.
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}