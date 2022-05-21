// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHealthbarWidget.h"
#include "Monster.h"
#include "Components/Progressbar.h"

void UMonsterHealthbarWidget::BindHP(class AMonster* Monster)
{
	CurrentMonster = Monster;
	Monster->OnHPChanged.AddUObject(this, &UMonsterHealthbarWidget::UpdateHP);
}

void UMonsterHealthbarWidget::UpdateHP()
{
	if (CurrentMonster.IsValid())
	{
		PB_HPBar->SetPercent(CurrentMonster->GetHPRatio());
	}
}

