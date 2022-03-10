// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UMainCharacterAnimInstance::UMainCharacterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK(TEXT("AnimMontage'/Game/_Game/Character/Animation/AttackMoNTAGE.AttackMontage'"));
	if (AM_ATTACK.Succeeded())
	{
		AttackMontage = AM_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_EVADE(TEXT("AnimMontage'/Game/_Game/Character/Animation/Evade.Evade'"));
	if (AM_EVADE.Succeeded())
	{
		EvadeMontage = AM_EVADE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_SKILL(TEXT("AnimMontage'/Game/_Game/Character/Animation/SkillMontage.SkillMontage'"));
	if (AM_SKILL.Succeeded())
	{
		SkillMontage = AM_SKILL.Object;
	}
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
		return;

	auto Character = Cast<AMainCharacter>(Pawn);
	if (Character)
	{
		CurrentPawnSpeed = Character->GetVelocity().Size();
		bIsInAir = Character->GetCharacterMovement()->IsFalling();
	}
	
}

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMainCharacterAnimInstance::PlayAttackMontage()
{
	if (AttackMontage)
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UMainCharacterAnimInstance::PlayEvadeMontage()
{
	if (EvadeMontage)
	{
		Montage_Play(EvadeMontage, 1.f);
	}
}

void UMainCharacterAnimInstance::JumpToSection(int32 MontageSection)
{
	FName Name = GetAttackMontageName(MontageSection);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UMainCharacterAnimInstance::GetAttackMontageName(int32 MontageSection)
{
	return FName(*FString::Printf(TEXT("Attack%d"), MontageSection));
}

void UMainCharacterAnimInstance::SetDeadAnim()
{
	bIsDead = true;
}

void UMainCharacterAnimInstance::RMBSkillMontagePlay()
{
	if (SkillMontage)
	{
		Montage_Play(SkillMontage, 1.f);
		Montage_JumpToSection(TEXT("RMBSkill"), SkillMontage);
	}
}

void UMainCharacterAnimInstance::AnimNotify_AttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackCheck"));
	OnAttackHitCheck.Broadcast();
}

void UMainCharacterAnimInstance::SetSprintingAnim()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
	}
	else
	{
		bIsSprinting = true;
	}
}


