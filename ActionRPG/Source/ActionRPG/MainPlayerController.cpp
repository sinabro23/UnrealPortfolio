// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MainGameplayWidget.h"
#include "PotionShopWidget.h"
#include "GameEndWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AMainPlayerController::AMainPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainGameplayWidget> UI_MENU_C(TEXT("WidgetBlueprint'/Game/_Game/UI/UI_Menu.UI_Menu_C'"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UPotionShopWidget> UI_POTIONSHOP_C(TEXT("WidgetBlueprint'/Game/_Game/UI/PotionShopWidget.PotionShopWidget_C'"));
	if (UI_POTIONSHOP_C.Succeeded())
	{
		PotionShopWidgetClass = UI_POTIONSHOP_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameEndWidget> UI_GAMEEND_C(TEXT("WidgetBlueprint'/Game/_Game/UI/GameEndWidget.GameEndWidget_C'"));
	if (UI_GAMEEND_C.Succeeded())
	{
		GameEndWidgetClass = UI_GAMEEND_C.Class;
	}


	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_BGM(TEXT("SoundCue'/Game/_Game/Assets/Sound/BGM/cinematic-dramatic-11120_Cue.cinematic-dramatic-11120_Cue'"));
	if (SC_BGM.Succeeded())
	{
		BGMSound = SC_BGM.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_BossBGM(TEXT("SoundCue'/Game/_Game/Assets/Sound/BGM/epic-dramatic-action-trailer-99525_Cue.epic-dramatic-action-trailer-99525_Cue'"));
	if (SC_BossBGM.Succeeded())
	{
		BossBGMSound = SC_BossBGM.Object;
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


	if (BossHUDOverlayAsset)
	{
		BossHUDOverlay = CreateWidget<UUserWidget>(this, BossHUDOverlayAsset);
	}

	if (BossHUDOverlay)
	{
		BossHUDOverlay->AddToViewport();
		BossHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}


	if (BGMSound)
	{
		AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), BGMSound);
	}
	
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AMainPlayerController::OnGamePause);
	//InputComponent->BindAction(TEXT("TestPotionShop"), EInputEvent::IE_Pressed, this, &AMainPlayerController::OnGameEnd);
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

void AMainPlayerController::OnPotionShop()
{
	PotionShopWidget = CreateWidget<UPotionShopWidget>(this, PotionShopWidgetClass);
	if (PotionShopWidget)
	{
		PotionShopWidget->AddToViewport(4);

		SetPause(true);
		ChangeInputMode(false);
	}
}

void AMainPlayerController::OnGameEnd()
{
	GameEndWidget = CreateWidget<UGameEndWidget>(this, GameEndWidgetClass);
	if (GameEndWidget)
	{
		GameEndWidget->AddToViewport(3);

		SetPause(true);
		ChangeInputMode(false);
	}
}

void AMainPlayerController::SetBossHPWidgetVisibility(bool bVisible)
{
	if (bVisible)
	{
		if (BossHUDOverlay)
		{
			BossHUDOverlay->AddToViewport();
			BossHUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (BossHUDOverlay)
		{
			BossHUDOverlay->AddToViewport();
			BossHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::StopOrdinaryBGM()
{
	AudioComp->Stop();
}

void AMainPlayerController::PlayBossRoomBGM()
{
	AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), BossBGMSound);
	AudioComp->Play();
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
