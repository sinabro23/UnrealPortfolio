// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY()
	class UUserWidget* HUDOverlay; //ȭ�鿡 ����� HUD��ü

public:


	void ChangeInputMode(bool bGameMode = true);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UMainGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UPotionShopWidget> PotionShopWidgetClass;

public:
	void OnGamePause();
	void OnPotionShop();


private:

	UPROPERTY()
	UMainGameplayWidget* MenuWidget;

	UPROPERTY()
	UPotionShopWidget* PotionShopWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
