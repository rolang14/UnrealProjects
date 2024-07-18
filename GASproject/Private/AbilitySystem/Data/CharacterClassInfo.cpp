// Copyright NHE


#include "AbilitySystem/Data/CharacterClassInfo.h"

const FCharacterClassDefaultInfo& UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass InCharacterClass) const
{
    return CharacterClassInfo.FindChecked(InCharacterClass);
}
