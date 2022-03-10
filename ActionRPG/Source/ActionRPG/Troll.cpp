// Fill out your copyright notice in the Description page of Project Settings.


#include "Troll.h"
#include "Monster.h"	

ATroll::ATroll()
{
	MonsterName = FString(TEXT("Troll"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_TROLL(TEXT("SkeletalMesh'/Game/_Game/Monster/Troll/Mesh/SK_Troll_Poison.SK_Troll_Poison'"));
	if (SK_TROLL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_TROLL.Object);
	}
	
	GetMesh()->SetRelativeScale3D(FVector(0.8));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.f), FRotator(0.0f, -90.f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> TrollAnim(TEXT("AnimBlueprint'/Game/_Game/Monster/Troll/Animation/BP_TrollAnim.BP_TrollAnim_C'"));
	if (TrollAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TrollAnim.Class);
	}

	SetAttackDamage(5.f);
}