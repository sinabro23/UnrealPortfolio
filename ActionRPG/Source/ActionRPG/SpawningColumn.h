// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawningColumn.generated.h"

UCLASS()
class ACTIONRPG_API ASpawningColumn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningColumn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UStaticMeshComponent* Mesh;

	int32 MonsterCount = 0;

	FTimerHandle SpawnTimerHandle;



	float DetectRange = 1200.f;

public:
	void MonsterCheck();

	FVector GetRandomPoint();

	int32 GetMonsterCount();

};
