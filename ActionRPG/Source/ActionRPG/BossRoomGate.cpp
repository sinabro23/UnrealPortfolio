// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomGate.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"

// Sets default values
ABossRoomGate::ABossRoomGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
}

// Called when the game starts or when spawned
void ABossRoomGate::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossRoomGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ABossRoomGate::OnBeginOverlap);

}

void ABossRoomGate::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetCharacterInBossRoom(true);
	}
}


