// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UMonsterNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindName(class AMonster* Monster);


private:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* MonsterName;

	TWeakObjectPtr<class AMonster> CurrentMonster;
};
