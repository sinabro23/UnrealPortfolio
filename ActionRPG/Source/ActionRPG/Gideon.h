// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gideon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

public:
	class UGideonAniminstance* GAnimInstance;
	class AGideonAIController* GideonAIController;


	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HPBar;

private:
	float MovementSpeed = 500.f;
	int32 AttackSectionIndex = 0;
public:
	void FireFireBall();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHP = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP = 500.f;
public:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SetHP(float NewHP);
	float GetHPRatio();

public:
	FOnHPChangedDelegate OnHPChanged;
	FOnAttackEndDelegate OnAttackEnd;
};
