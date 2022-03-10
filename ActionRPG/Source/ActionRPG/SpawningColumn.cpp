// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningColumn.h"
#include "Components/SphereComponent.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Troll.h"
#include "Grunt.h"
#include "NavigationSystem.h"

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

	if (!IsSpawnOn)
		return;

	MonsterCheck();

	if (MonsterCount <= 0)
	{
		MonsterCount = 0;
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
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

		int32 Count = 0;
		for (auto OverlapResult : OverlapResults)
		{
			if (OverlapResult.Actor.IsValid())
			{
				Count++;
			}
			
		}
		MonsterCount = Count;

	
	}
	else
	{
		MonsterCount = 0;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

			SpawnMonster();
			SpawnMonster();

		}), 2.f, false);
	}

	DrawDebugSphere(GetWorld(), Center, DetectRange, 16, FColor::Blue, false, 1.f);
}

FVector ASpawningColumn::GetRandomPoint()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return FVector::ZeroVector;

	FNavLocation RandomLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 500.f, RandomLocation))
	{
		return RandomLocation;
	}
	else
	{
		return FVector::ZeroVector;
	}

}

int32 ASpawningColumn::GetMonsterCount()
{
	return MonsterCount;
}

void ASpawningColumn::SpawnMonster()
{
	ATroll* Troll = GetWorld()->SpawnActor<ATroll>(GetRandomPoint(), FRotator::ZeroRotator);
	AGrunt* Grunt = GetWorld()->SpawnActor<AGrunt>(GetRandomPoint(), FRotator::ZeroRotator);
	
}

