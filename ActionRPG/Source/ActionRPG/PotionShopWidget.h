// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PotionShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UPotionShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHPPotionButtonClicked();
	UFUNCTION()
	void OnMPPotionButtonClicked();
	UFUNCTION()
	void OnReturnButtonClicked();

protected:
	UPROPERTY()
	class UButton* HPPotion;
	UPROPERTY()
	UButton* MPPotion;
	UPROPERTY()
	UButton* ReturnButton;
};
