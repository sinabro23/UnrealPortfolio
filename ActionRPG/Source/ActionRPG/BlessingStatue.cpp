// Fill out your copyright notice in the Description page of Project Settings.


#include "BlessingStatue.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MainCharacter.h"

// Sets default values
ABlessingStatue::ABlessingStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_STATUE(TEXT("StaticMesh'/Game/_Game/Background/SM_Plains_Angel_Statue_01.SM_Plains_Angel_Statue_01'"));
	if (SM_STATUE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_STATUE.Object);
	}

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->InitSphereRadius(300.f);

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
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABlessingStatue::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABlessingStatue::OnOverlapEnd);

	TextWidget->SetVisibility(false);
}

// Called every frame
void ABlessingStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlessingStatue::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		TextWidget->SetVisibility(true);
	}
}

void ABlessingStatue::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		TextWidget->SetVisibility(false);
	}
}

