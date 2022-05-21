// Fill out your copyright notice in the Description page of Project Settings.


#include "BlessingStatue.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ABlessingStatue::ABlessingStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_STATUE(TEXT("StaticMesh'/Game/_Game/Background/SM_Plains_Angel_Statue_01.SM_Plains_Angel_Statue_01'"));
	if (SM_STATUE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_STATUE.Object);
	}

	TextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TEXTWIDGET"));
	TextWidget->SetupAttachment(GetRootComponent());
	TextWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TextWidget->bAutoActivate = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> UW_BLESSTEXT(TEXT("WidgetBlueprint'/Game/_Game/UI/ETC/BleesingText.BleesingText_C'"));
	if (UW_BLESSTEXT.Succeeded())
	{
		TextWidget->SetWidgetClass(UW_BLESSTEXT.Class);
		TextWidget->SetDrawSize(FVector2D(500.f, 100.f));
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetupAttachment(GetRootComponent());
	Trigger->SetBoxExtent(FVector(400.f, 400.f, 200.f));
	Trigger->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ABlessingStatue::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlessingStatue::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TextWidget->InitWidget();
	TextWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));

	TextWidget->SetVisibility(false);
}

void ABlessingStatue::TurnOnWidget()
{
	TextWidget->SetVisibility(true);
}

void ABlessingStatue::TurnOffWidget()
{
	TextWidget->SetVisibility(false);
}


