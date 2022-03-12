// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gideon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class ACTIONRPG_API AGideon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGideon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

public:
	class UGideonAniminstance* GAnimInstance;
	class AGideonAIController* GideonAIController;

private:
	float MovementSpeed = 500.f;
	int32 AttackSectionIndex = 0;
public:
	void FireBall();

public:
	FOnAttackEndDelegate OnAttackEnd;


public:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
