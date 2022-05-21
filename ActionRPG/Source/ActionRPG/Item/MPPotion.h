// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPPotion.generated.h"

UCLASS()
class ACTIONRPG_API AMPPotion : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMPPotion();

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* HPPotionParticle;

	UPROPERTY(VisibleAnywhere, Category = Box)
	class UBoxComponent* Trigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
