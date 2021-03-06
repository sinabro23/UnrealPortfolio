// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBall.generated.h"

UCLASS()
class ACTIONRPG_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* FireballSphere;

	UPROPERTY(VisibleAnywhere, Category = FX)
	UParticleSystemComponent* FireballFX;

	UPROPERTY(VisibleAnywhere, Category = FX)
	UParticleSystem* HitEffect;
	
	UPROPERTY()
	float speed;

	UPROPERTY()
	FVector CurrentLocation;

	UPROPERTY()
	FVector OriginVector;

	UPROPERTY()
	class AGideon* FireballOwner;

	UPROPERTY()
	FTimerHandle DeadTimer;
public:
	void SetFireballOwner(AGideon* Gideon);

	void SetFireballRotation(FRotator Rotator);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
