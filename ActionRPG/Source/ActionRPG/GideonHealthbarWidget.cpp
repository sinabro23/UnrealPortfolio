// Fill out your copyright notice in the Description page of Project Settings.


#include "GideonHealthbarWidget.h"
#include "Gideon.h"
#include "Components/Progressbar.h"

void UGideonHealthbarWidget::BindHP(AGideon* Gideon)
{
	GideonPtr = Gideon;
	GideonPtr->OnHPChanged.AddUObject(this, &UGideonHealthbarWidget::UpdateHP);
}

void UGideonHealthbarWidget::UpdateHP()
{
	if (GideonPtr.IsValid())
	{
		PB_GIDEONHP->SetPercent(GideonPtr->GetHPRatio());
	}
}
