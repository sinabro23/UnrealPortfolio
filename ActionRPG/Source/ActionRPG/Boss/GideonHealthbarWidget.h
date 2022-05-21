// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GideonHealthbarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UGideonHealthbarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHP(class AGideon* Gideon);

	void UpdateHP();


private:
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* PB_GIDEONHP;

	TWeakObjectPtr<AGideon> GideonPtr;
};
