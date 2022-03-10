// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningColumn.h"
#include "Components/SphereComponent.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "DrawDebugHelpers.h"
// Sets default values
ASpawningColumn::ASpawningColumn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(3.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_COLUMN(TEXT("StaticMesh'/Game/_Game/Background/SM_Plains_Column_Large_01.SM_Plains_Column_Large_01'"));
	if (SM_COLUMN.Succeeded())
	{
		Mesh->SetStaticMesh(SM_COLUMN.Object);
	}

}

// Called when the game starts or when spawned
void ASpawningColumn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawningColumn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

// Called every frame
void ASpawningColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MonsterCheck();
	
	UE_LOG(LogTemp, Warning, TEXT("Monster Count : %d"), MonsterCount);
}

void ASpawningColumn::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMonster* Monster = Cast<AMonster>(OtherActor);
	if (Monster)
	{
		MonsterCount++;
	}

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("MONSTER COUNT : %d"), MonsterCount);
	}
}

void ASpawningColumn::MonsterCheck()
{
	FVector Center = GetActorLocation();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollsionQueryParam(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRange),
		CollsionQueryParam
	);

	if (bResult)
	{
		int32 Count = 0;
		for (auto OverlapResult : OverlapResults)
		{
			if (OverlapResult.Actor.IsValid())
			{
			//	UE_LOG(LogTemp, Warning, TEXT("Monster Exist"));
				Count++;
			}
		}

		MonsterCount = Count;
	}
}

int32 ASpawningColumn::GetMonsterCount()
{
	return MonsterCount;
}

