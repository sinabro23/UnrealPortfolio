// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gideon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBossDeadDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTransfromEndDelegate);
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

	class AGideonAIController* MyAiController;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* LockOnParticle;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* FireAura;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* FireSpin;

	//UPROPERTY(VisibleAnywhere)
	//class UWidgetComponent* HPBar;

	FTimerHandle DeadTimerHandle;

	class AMainCharacter* MainCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName BossName = FName("Gideon");

	FTimerHandle SecondPageMeteorTimer;
	float MeteorTime = 0.5f;
	int32 MeteorRepeatCount = 0;

	FTimerHandle GetMeteorVectorTimer;
	float GetMeteorVectorRepeatTime = 0.5f;
	int32 GetMeteorVectorRepeatCount = 0;

	FTimerHandle MeteorOneShotTimer;
	float MeteorOneShotTime = 0.5f;

private:
	float MovementSpeed = 500.f;
	int32 AttackSectionIndex = 0;

	bool bIsDead = false;

	bool bLockOnVisible = false;

	bool bCanbeAttacked = true;
	TWeakObjectPtr<class AMainCharacter> MeteorTarget;
	
	UPROPERTY()
	FVector MeteorVector;

	UPROPERTY()
	UParticleSystem* MeteorCastParitle;
	UPROPERTY()
	UParticleSystem* MeteorFireParticle;
	UPROPERTY()
	UParticleSystem* MeteorShowerParticle;

	FVector MeteorSpawnVector = FVector();

	class USoundCue* FireballSound;
	USoundCue* MeteorSound;


public:
	void FireFireBall();
	void FireMeteor();
	
	void FireFireBall2();
	
	bool IsDead();

	void MeteorCast();
	void MeteorFire();
	void TransformEffect();

	void TransformPage2();

	void SetCanBeAttacked(bool CanBe);

	void RepeatMeteor();

	void SendMeteorRepeat();
	void SendMeteorOneShot();
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

	void Death();

	void SetHPBarVisiblity(bool Visibility);

	void LockOn();
	void LockOff();
public:
	FOnHPChangedDelegate OnHPChanged;
	FOnAttackEndDelegate OnAttackEnd;
	FOnBossDeadDelegate OnBossDead;
	FOnTransfromEndDelegate OnTransformEndGideon;
};
