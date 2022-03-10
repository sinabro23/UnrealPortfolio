// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UMainGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeClicked();
	UFUNCTION()
	void OnReturnToTitleClicked();
	UFUNCTION()
	void OnRetryGameClicked();

protected:
	UPROPERTY()
	class UButton* ResumeButton;
	UPROPERTY()
	UButton* ReturnToTitleButton;
	UPROPERTY()
	UButton* RetryGameButton;
};
