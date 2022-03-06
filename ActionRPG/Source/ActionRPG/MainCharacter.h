// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class ACTIONRPG_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = true))
	class UMainCharacterAnimInstance* MainAnim;

private:
	int32 AttackSectionIndex = 0;
	bool bIsAttacking = false;
public:
	void UpDown(float InputValue);
	void LeftRight(float InputValue);

	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	// ���� ����
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentMP = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxMP = 100.f;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHPRatio() { return CurrentHP / MaxHP; }

};
