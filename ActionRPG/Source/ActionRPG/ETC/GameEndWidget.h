// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnButtonClicked();

protected:
	UPROPERTY()
	class UButton* ReturnButton;
};
