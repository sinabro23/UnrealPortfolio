// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"
#include "Components/Button.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UGameEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnButton = Cast<UButton>(GetWidgetFromName(TEXT("ReturnToTitleButton")));
	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UGameEndWidget::OnReturnButtonClicked);
	}
}

void UGameEndWidget::OnReturnButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
