// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNameWidget.h"
#include "Components/TextBlock.h"
#include "Monster.h"

void UMonsterNameWidget::BindName(class AMonster* Monster)
{
	CurrentMonster = Monster;

	if (CurrentMonster.IsValid())
	{
		MonsterName->SetText(FText::FromString(CurrentMonster->GetMonsterName()));
	}
}