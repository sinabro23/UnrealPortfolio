// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

ABear::ABear()
{
	MonsterName = FString(TEXT("Bear"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BEAR(TEXT("SkeletalMesh'/Game/_Game/Monster/Bear/Mesh/Enemy_Bear.Enemy_Bear'"));
	if (SK_BEAR.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BEAR.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -120.f), FRotator(0.0f, -90.f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	HPBarHeight = 200.f;

	static ConstructorHelpers::FClassFinder<UAnimInstance> BearAnim(TEXT("AnimBlueprint'/Game/_Game/Monster/Bear/Animations/BearAnim.BearAnim_C'"));
	if (BearAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BearAnim.Class);
	}

	GetCapsuleComponent()->SetCapsuleRadius(120.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);


	SetAttackDamage(9.f);
	SetMovementSpeed(600.f);
	SetMaxHP(240.f);
	SetHP(MaxHP);
}