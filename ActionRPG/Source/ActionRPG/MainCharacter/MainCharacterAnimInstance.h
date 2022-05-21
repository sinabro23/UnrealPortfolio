// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate)
/**
 * 
 */
UCLASS()
class ACTIONRPG_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* EvadeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsSprinting = false;

public:
	void PlayAttackMontage();
	void PlayEvadeMontage();
	void JumpToSection(int32 MontageSection);
	FName GetAttackMontageName(int32 MontageSection);

	void SetDeadAnim();

	void RMBSkillMontagePlay();
	void RSkillMontagePlay();
	UFUNCTION()
	void AnimNotify_AttackCheck();

	void SetSprintingAnim();

public:
	FOnAttackHitCheckDelegate OnAttackHitCheck;
};
