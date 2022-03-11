// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionShopWidget.h"
#include "Components/Button.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

void UPotionShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HPPotionButton = Cast<UButton>(GetWidgetFromName(TEXT("HPPotionbtn")));
	if (HPPotionButton)
	{
		HPPotionButton->OnClicked.AddDynamic(this, &UPotionShopWidget::OnHPPotionButtonClicked);
	}

	MPPotionButton = Cast<UButton>(GetWidgetFromName(TEXT("MPPotionbtn")));
	if (MPPotionButton)
	{
		MPPotionButton->OnClicked.AddDynamic(this, &UPotionShopWidget::OnMPPotionButtonClicked);
	}

	ReturnButton = Cast<UButton>(GetWidgetFromName(TEXT("Exitbtn")));
	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UPotionShopWidget::OnReturnButtonClicked);
	}
}

void UPotionShopWidget::OnHPPotionButtonClicked()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		Character->GetHPPotion();
	}
}

void UPotionShopWidget::OnMPPotionButtonClicked()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		Character->GetMPPotion();
	}
}

void UPotionShopWidget::OnReturnButtonClicked()
{
	auto MainController = Cast<AMainPlayerController>(GetOwningPlayer());
	if (MainController)
	{
		RemoveFromParent();
		MainController->ChangeInputMode(true);
		MainController->SetPause(false);
	}
}
