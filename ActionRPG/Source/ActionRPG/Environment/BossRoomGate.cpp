// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomGate.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Gideon.h"

// Sets default values
ABossRoomGate::ABossRoomGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(TEXT("StaticMesh'/Game/Infinity_Blade_Assets/Meshes/Props/WroughtlronGate.WroughtlronGate'"));
	if (SM_GATE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_GATE.Object);
	}

	RootComponent = Mesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	Box->SetBoxExtent(FVector(80.f, 80.f, 30.f));
	Box->SetupAttachment(GetRootComponent());
	Box->SetCollisionProfileName(TEXT("Item"));

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_ROLLING(TEXT("SoundCue'/Game/_Game/Assets/Sound/RockRolling__online-audio-converter_com__Cue.RockRolling__online-audio-converter_com__Cue'"));
	if (SC_ROLLING.Succeeded())
	{
		RollingSound = SC_ROLLING.Object;
	}
}

// Called when the game starts or when spawned
void ABossRoomGate::BeginPlay()
{
	Super::BeginPlay();

	FirstPositon = GetActorLocation();
}

void ABossRoomGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ABossRoomGate::OnBeginOverlap);

}

void ABossRoomGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsInBoosRoom)
	{
		SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, DeltaTime * 450.f));
	}
}

void ABossRoomGate::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetCharacterInBossRoom(true);
		AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(Character->GetController());
		if (MainPlayerController)
		{
			MainPlayerController->SetBossHPWidgetVisibility(true);
			MainPlayerController->StopOrdinaryBGM();
			MainPlayerController->PlayBossRoomBGM();
		}
		IsInBoosRoom = true;

		UGameplayStatics::PlaySound2D(GetWorld(), RollingSound);
		Character->GetBoss()->PlayOpeningSound();
		GetWorldTimerManager().SetTimer(GateTimer, this, &ABossRoomGate::GateClosed, GateEndTime);
	}
}

void ABossRoomGate::GateClosed()
{
	IsInBoosRoom = false;
}


