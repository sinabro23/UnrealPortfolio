// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GideonAniminstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate)
/**
 * 
 */
UCLASS()
class ACTIONRPG_API UGideonAniminstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGideonAniminstance();

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	void SetDeadAnim();

	void PlayAttackMontage();
	void JumpToSection(int32 MontageSection);
	FName GetAttackMontageName(int32 MontageSection);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

public:
	FOnAttackHitCheckDelegate OnAttackHitCheck;
};
