// Fill out your copyright notice in the Description page of Project Settings.


#include "GideonAniminstance.h"
#include "BossGidon.h"

UGideonAniminstance::UGideonAniminstance()
{

}



void UGideonAniminstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
		return;

	auto Gideon = Cast<ABossGidon>(Pawn);
	if (Gideon)
	{
		CurrentPawnSpeed = Gideon->GetVelocity().Size();
	}
}

void UGideonAniminstance::SetDeadAnim()
{
}

void UGideonAniminstance::PlayAttackMontage()
{
}

