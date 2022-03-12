// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_NORMAL UMETA(DisplayName = "Normal"),
	EMS_SPRINTING UMETA(DisplayName = "Sprinting"),

	EMS_MAX UMETA(DisplayName = "Max")

};

UCLASS()
class ACTIONRPG_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	void SetMovementStatus(EMovementStatus NewStatus);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed = 650.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SpringtingSpeed = 950.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Running")
	float StaminaDrainRate = 25.f; // 뛸때 스태미나 줄어드는 양

	void CapslockKeyDown();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
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

	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* RMBSkillParticle;

	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UParticleSystem* RMBSkillFX;

	UPROPERTY(VisibleAnywhere, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* BuffParticle;


private:
	int32 AttackSectionIndex = 0;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	bool CharacterCanBeDamaged = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsLockOn = false;

	float LockOnRange = 700.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class AMonster> CurrentTargetMonster;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FRotator LockOnLookAtRotation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class USphereComponent* SphereForTargetHPBar;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class USphereComponent* NearRangeSphere;
	bool bIsNearSphereOverlapped = false;

	FTimerHandle DeadTimerHandle;

	bool bIsDead = false;

	bool bCanBuy = false;

	bool bIsInBossRoom = false;
public:
	bool IsCharacterInBossRoom();
	void SetCharacterInBossRoom(bool IsIn);
protected:
	// 카메라 줌 관련
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

	void EKeyPressed();
	void EKeyReleased();
	bool IsEkeyPressed = false;

	void LockOn();

	void Blessed();

	void RMBSkill();

	void Dead();

	void HPPotion();
	void MPPotion();
	void GetHPPotion();
	void GetMPPotion();
	
	void SetCanBuy(bool Input);
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	FRotator GetLookAtRotationYaw(FVector TargetVector);

	UFUNCTION()
	void OnSphereOverlappedForHPBar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlappedForHPBar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnSphereOverlappedForNear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlappedForNear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// 스탯 관련
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentMP = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxMP = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float CurrentStamina = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	int32 HPPotionCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	int32 MPPotionCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stat", Meta = (AllowPrivateAccess = true))
	int32 Coins = 1000;

	float AttackDamage = 40.f;
	float RMBSkillDamage = 50.f;
	float RMBSkillRange = 800.f;
	float HPPotionHealth = 30.f;
	float MPPotionHealth = 30.f;

public:

	void SetHP(float NewHP);
	void SetMP(float NewMP);
	void GetCoins(int32 NewCoin);
	int32 GetCoinCounts();
	void SetCoins(int32 NewCoin);

	float GetCurrentHP();


public:
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHPRatio() { return CurrentHP / MaxHP; }


};
