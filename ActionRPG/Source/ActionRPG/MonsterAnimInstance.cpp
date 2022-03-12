// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK(TEXT("AnimMontage'/Game/_Game/Monster/Troll/Animation/AttackMontage.AttackMontage'"));
	if (AM_ATTACK.Succeeded())
	{
		AttackMontage = AM_ATTACK.Object;
	}
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
		return;

	auto Monster = Cast<AMonster>(Pawn);
	if (Monster)
	{
		CurrentPawnSpeed = Monster->GetVelocity().Size();
	}
}

void UMonsterAnimInstance::SetDeadAnim()
{
	bIsDead = true;
}


void UMonsterAnimInstance::PlayAttackMontage()
{
	if (AttackMontage)
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UMonsterAnimInstance::JumpToSection(int32 MontageSection)
{
	FName Name = GetAttackMontageName(MontageSection);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UMonsterAnimInstance::GetAttackMontageName(int32 MontageSection)
{
	return FName(*FString::Printf(TEXT("Attack%d"), MontageSection));
}

void UMonsterAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackHitCheck.Broadcast();
}

