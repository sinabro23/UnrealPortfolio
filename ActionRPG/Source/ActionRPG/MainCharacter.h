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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void AddControllerYawInput(float Val) override;
	virtual void Jump()override;

private:

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = true))
	class UMainCharacterAnimInstance* MainAnim;

	UPROPERTY(VisibleAnywhere, Category = Sound, Meta = (AllowPrivateAccess = true))
	class USoundCue* AttackSound;


private:
	int32 AttackSectionIndex = 0;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	bool CharacterCanBeDamaged = true;

	bool bIsLockOn = false;
	float LockOnRange = 700.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class AMonster> CurrentTargetMonster;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FRotator LockOnLookAtRotation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class USphereComponent* SphereForTargetHPBar;

	FTimerHandle DeadTimerHandle;

	bool bIsDead = false;
protected:
	// Ä«¸Þ¶ó ÁÜ °ü·Ã
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MinSpringArmLength = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MaxSpringArmLength = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float DeafaultSpinrgArmLength = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ZoomStep = 40.f;

public:
	void UpDown(float InputValue);
	void LeftRight(float InputValue);
	void CameraZoom(float InputValue);

	void Attack();
	void AttackHitCheck();
	void ShiftKey();

	void LockOn();

	void Dead();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	FRotator GetLookAtRotationYaw(FVector TargetVector);

	UFUNCTION()
	void OnSphereOverlappedForHPBar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlappedForHPBar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// ½ºÅÈ °ü·Ã
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentMP = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxMP = 100.f;

	float AttackDamage = 5.f;


public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHPRatio() { return CurrentHP / MaxHP; }

};
