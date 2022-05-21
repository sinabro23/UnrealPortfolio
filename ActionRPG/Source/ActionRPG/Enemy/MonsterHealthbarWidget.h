// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHealthbarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UMonsterHealthbarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHP(class AMonster* Monster);

	void UpdateHP();


private:
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* PB_HPBar;

	TWeakObjectPtr<class AMonster> CurrentMonster;

};
