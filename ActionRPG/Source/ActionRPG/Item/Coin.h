// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class ACTIONRPG_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	int32 CoinAmount = 100.f;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* CoinParticle;

	UPROPERTY(VisibleAnywhere, Category = Box)
	class UBoxComponent* Trigger;

public:
	void SetCoinAmount(int32 NewCoinAmount);

public:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
