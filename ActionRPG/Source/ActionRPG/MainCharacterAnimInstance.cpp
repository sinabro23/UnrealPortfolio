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
