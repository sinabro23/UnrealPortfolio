// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Gideon.h"
#include "MainCharacter.h"
// Sets default values
AFireBall::AFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireballSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FIREBALLSPHERE"));
	FireballSphere->InitSphereRadius(15.f);
	FireballSphere->SetCollisionProfileName(TEXT("Item"));
	FireballSphere->SetHiddenInGame(false);
	RootComponent = FireballSphere;

	FireballFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FIREBALLFX"));
	FireballFX->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FIREBALL(TEXT("ParticleSystem'/Game/_Game/Gideon/FX/P_FireBall_Strong.P_FireBall_Strong'"));
	if (PS_FIREBALL.Succeeded())
	{
		FireballFX->SetTemplate(PS_FIREBALL.Object);
	}
}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	OriginVector = GetActorLocation();
	CurrentLocation = GetActorLocation(); // To save where ever the actor is in the viewport
	speed = 1000.0f;
	
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([this]()-> void {

		Destroy();

	}), 7.f, false);
}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CurrentLocation += GetActorForwardVector() * speed * DeltaTime; // use X Y or Z.  Use -= to go the opposite way

	SetActorLocation(CurrentLocation);

	if ((CurrentLocation - OriginVector).Size() >= 5000.f)
	{
		Destroy();
	}

}

void AFireBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FireballSphere->OnComponentBeginOverlap.AddDynamic(this, &AFireBall::OnBeginOverlap);
}


void AFireBall::SetFireballOwner(AGideon* Gideon)
{
	FireballOwner = Gideon;
}

void AFireBall::SetFireballRotation(FRotator Rotator)
{
	SetActorRotation(Rotator);
}

void AFireBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetHP(Character->GetCurrentHP() - 15.f);
	}

	Destroy();
}

