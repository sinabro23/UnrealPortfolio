// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlessingStatue.generated.h"

UCLASS()
class ACTIONRPG_API ABlessingStatue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlessingStatue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


public:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* TextWidget;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;

public:

	void TurnOnWidget();
	void TurnOffWidget();
};
