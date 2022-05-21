// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brazier.generated.h"

UCLASS()
class ACTIONRPG_API ABrazier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrazier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* FireFX;

};
