// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionShop.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APotionShop::APotionShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(200.f, 300.f, 100.f));
	Trigger->SetCollisionProfileName(TEXT("Item"));
	RootComponent = Trigger;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Mesh->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_POTIONTABLE(TEXT("StaticMesh'/Game/_Game/Shop/Table.Table'"));
	if (SM_POTIONTABLE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_POTIONTABLE.Object);
	}

	TextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TEXTWIDGET"));
	TextWidget->SetupAttachment(GetRootComponent());
	TextWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TextWidget->bAutoActivate = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> UW_SELLING(TEXT("WidgetBlueprint'/Game/_Game/UI/ShopTextWidget.ShopTextWidget_C'"));
	if (UW_SELLING.Succeeded())
	{
		TextWidget->SetWidgetClass(UW_SELLING.Class);
		TextWidget->SetDrawSize(FVector2D(500.f, 100.f));
	}


}

// Called when the game starts or when spawned
void APotionShop::BeginPlay()
{
	Super::BeginPlay();
	
}

void APotionShop::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APotionShop::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &APotionShop::OnOverlapEnd);

	TextWidget->InitWidget();
	TextWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	TextWidget->SetVisibility(false);
}

// Called every frame
void APotionShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APotionShop::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetCanBuy(true);
		TextWidget->SetVisibility(true);
	}
}

void APotionShop::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetCanBuy(false);
		TextWidget->SetVisibility(false);
	}
}

