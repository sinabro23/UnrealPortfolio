// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MainGameplayWidget.h"
AMainPlayerController::AMainPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainGameplayWidget> UI_MENU_C(TEXT("WidgetBlueprint'/Game/_Game/UI/UI_Menu.UI_Menu_C'"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	ChangeInputMode(true);

}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AMainPlayerController::OnGamePause);
}

void AMainPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UMainGameplayWidget>(this, MenuWidgetClass);
	if (MenuWidget)
	{
		MenuWidget->AddToViewport(3);

		SetPause(true);
		ChangeInputMode(false);
	}
}

void AMainPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}
