// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGERR"));
	Trigger->SetBoxExtent(FVector(50.f, 50.f, 5.f));
	Trigger->SetCollisionProfileName(FName(TEXT("Item")));
	Trigger->SetHiddenInGame(false);
	RootComponent = Trigger;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Mesh->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_COIN(TEXT("StaticMesh'/Game/_Game/Background/SM_Treasure_Coin.SM_Treasure_Coin'"));
	if (SM_COIN.Succeeded())
	{
		Mesh->SetStaticMesh(SM_COIN.Object);
	}

	CoinParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("COINPARTICLE"));
	CoinParticle->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_COIN(TEXT("ParticleSystem'/Game/_Game/FX/P_TreasureChest_CallOut_loop_Tier1.P_TreasureChest_CallOut_loop_Tier1'"));
	if (PS_COIN.Succeeded())
	{
		CoinParticle->SetTemplate(PS_COIN.Object);
	}
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoin::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnCharacterOverlap);
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoin::SetCoinAmount(int32 NewCoinAmount)
{
	CoinAmount = NewCoinAmount;
}

void ACoin::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);

	if (Character)
	{
		Character->GetCoins(CoinAmount);
		Destroy();
	}
}

