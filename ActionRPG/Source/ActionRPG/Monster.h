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

public:
	UPROPERTY()
	class AMonsterAIController* MonsterAIController;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HPBar;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP = 50.f;
	// �ǰ��õǴ°� ������ SetHP�� ȣ��
public:
	void SetHP(float NewHP);
	float GetHPRatio();

public:
	FOnHPChangedDelegate OnHPChanged;
};
