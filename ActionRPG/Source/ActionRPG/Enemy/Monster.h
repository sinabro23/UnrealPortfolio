// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class ACoin;
UCLASS()
class ACTIONRPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY()
	class AMonsterAIController* MonsterAIController;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HPBar;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* NameBox;

	UPROPERTY(VisibleAnywhere)
	class UMonsterAnimInstance* MonsterAnim;

	FTimerHandle DeadTimerHandle = {};

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ExclamationMark;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BloodParitcle;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* LockOnParticle;
	
	UPROPERTY(EditDefaultsOnly)
	float ExclamationMarkHeight = 100.f;

	UPROPERTY(EditDefaultsOnly)
	FString MonsterName;

	bool bIsAttacking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(EditDefaultsOnly, Category = WIDGET)
	float HPBarHeight = 300.f;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float AttackDamage = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MovementSpeed = 10.f;

	void SetAttackDamage(float NewDamage);
	void SetMovementSpeed(float NewSpeed);
	void SetMaxHP(float NewHP);
	int32 RemainCoinAmount = 0;
	void SetRemainCoinAmount(int32 Amount);
	// ???????????? ?????? SetHP?? ????
public:
	void SetHP(float NewHP);
	float GetHPRatio();

	void FindEnemy();
	void MissEnemy();

	void SpawnItem();

	void Dead();
	bool bIsDead = false;
	bool IsDead();

	bool ShouldHappenInPercent(int percentage);

	UFUNCTION(BlueprintCallable)
	FString GetMonsterName();

	void Attack();
	int32 AttackSectionIndex = 0;

	void AttackHitCheck();

	bool bLockOnVisible = false;
	void LockOn();
	void LockOff();

	void WidgetTurnOn();
	void WidgetTurnOff();

	FVector OriginVector;
	FVector GetOriginVector();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	FOnHPChangedDelegate OnHPChanged;
	FOnAttackEndDelegate OnAttackEnd;
};
