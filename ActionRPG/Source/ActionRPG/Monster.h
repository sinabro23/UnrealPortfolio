// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate)

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
	
	UPROPERTY(EditDefaultsOnly)
	float ExclamationMarkHeight = 100.f;

	UPROPERTY(EditDefaultsOnly)
	FString MonsterName;

protected:
	


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP = 50.f;
	// 피관련되는건 무조건 SetHP로 호출
public:
	void SetHP(float NewHP);
	float GetHPRatio();

	void FindEnemy();
	void MissEnemy();

	void Dead();

	UFUNCTION(BlueprintCallable)
	FString GetMonsterName();

public:
	FOnHPChangedDelegate OnHPChanged;
};
