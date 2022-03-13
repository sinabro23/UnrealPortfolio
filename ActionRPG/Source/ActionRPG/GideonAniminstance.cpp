// Fill out your copyright notice in the Description page of Project Settings.


#include "GideonAniminstance.h"
#include "Gideon.h"
UGideonAniminstance::UGideonAniminstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_GIDEONATTACK(TEXT("AnimMontage'/Game/_Game/Gideon/Animation/AttackMontage.AttackMontage'"));
	if (AM_GIDEONATTACK.Succeeded())
	{
		AttackMontage = AM_GIDEONATTACK.Object;
	}
}

void UGideonAniminstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
		return;

	auto Gideon = Cast<AGideon>(Pawn);
	if (Gideon)
	{
		CurrentPawnSpeed = Gideon->GetVelocity().Size();
	}
}

void UGideonAniminstance::SetDeadAnim()
{
	bIsDead = true;
}

void UGideonAniminstance::PlayAttackMontage()
{
	if (AttackMontage)
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UGideonAniminstance::PlayMeteorMontage()
{
	if (AttackMontage)
	{
		Montage_Play(AttackMontage, 1.f);
		Montage_JumpToSection(FName("Meteor"));
	}
}

void UGideonAniminstance::JumpToSection(int32 MontageSection)
{
	FName Name = GetAttackMontageName(MontageSection);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UGideonAniminstance::GetAttackMontageName(int32 MontageSection)
{
	return FName(*FString::Printf(TEXT("Attack%d"), MontageSection));
}

void UGideonAniminstance::AnimNotify_MeteorCast()
{
	OnMeteorCast.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("METEORCAST"));
}

void UGideonAniminstance::AnimNotify_MeteorFire()
{
	onFireMeteor.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("MeteorFire"));
}

