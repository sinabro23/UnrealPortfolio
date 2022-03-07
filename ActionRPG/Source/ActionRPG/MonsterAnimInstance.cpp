// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{

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
