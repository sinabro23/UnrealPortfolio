// Fill out your copyright notice in the Description page of Project Settings.


#include "MPPotion.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMPPotion::AMPPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetupAttachment(GetRootComponent());
	RootComponent = Trigger;
	Trigger->SetBoxExtent(FVector(30.f, 36.f, 22.f));
	Trigger->SetCollisionProfileName(FName(TEXT("Item")));
	Trigger->SetHiddenInGame(false);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_HEALTHPOTION(TEXT("StaticMesh'/Game/_Game/Background/Potion_3.Potion_3'"));
	if (SM_HEALTHPOTION.Succeeded())
	{
		Mesh->SetStaticMesh(SM_HEALTHPOTION.Object);
	}

	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -11.f));

	HPPotionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HPPOTIONPARTICLE"));
	HPPotionParticle->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_MPPOTION(TEXT("ParticleSystem'/Game/_Game/FX/P_Status_Magic_Glow.P_Status_Magic_Glow'"));
	if (PS_MPPOTION.Succeeded())
	{
		HPPotionParticle->SetTemplate(PS_MPPOTION.Object);
	}
}

// Called when the game starts or when spawned
void AMPPotion::BeginPlay()
{
	Super::BeginPlay();

}

void AMPPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMPPotion::OnCharacterOverlap);
}

void AMPPotion::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->GetMPPotion();
		Destroy();
	}
}

