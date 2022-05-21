// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameplayWidget.h"
#include "Components/Button.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UMainGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UMainGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UMainGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast <UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UMainGameplayWidget::OnRetryGameClicked);
	}

}

void UMainGameplayWidget::OnResumeClicked()
{
	auto MainController = Cast<AMainPlayerController>(GetOwningPlayer());
	if (MainController)
	{
		RemoveFromParent();
		MainController->ChangeInputMode(true);
		MainController->SetPause(false);
	}
}

void UMainGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UMainGameplayWidget::OnRetryGameClicked()
{
	auto MainController = Cast<AMainPlayerController>(GetOwningPlayer());
	if (MainController)
	{
		MainController->RestartLevel();
	}
}
