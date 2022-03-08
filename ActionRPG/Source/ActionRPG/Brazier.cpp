// Fill out your copyright notice in the Description page of Project Settings.


#include "Brazier.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABrazier::ABrazier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BRAZIER(TEXT("StaticMesh'/Game/_Game/Background/SM_Plains_Castle_Fountain_02.SM_Plains_Castle_Fountain_02'"));
	if (SM_BRAZIER.Succeeded())
	{
		Mesh->SetStaticMesh(SM_BRAZIER.Object);
	}

	FireFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FIREFX"));
	FireFX->SetupAttachment(RootComponent);
	FireFX->SetRelativeLocation(FVector(0.0f, 0.0f, 240.f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FIRE(TEXT("ParticleSystem'/Game/_Game/Background/P_Buff_JumpDiveFire_02.P_Buff_JumpDiveFire_02'"));
	if (PS_FIRE.Succeeded())
	{
		FireFX->SetTemplate(PS_FIRE.Object);
	}
}

// Called when the game starts or when spawned
void ABrazier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrazier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

