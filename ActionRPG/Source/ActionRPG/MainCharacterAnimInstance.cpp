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

void UMainCharacterAnimInstance::AnimNotify_AttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackCheck"));
	OnAttackHitCheck.Broadcast();
}


