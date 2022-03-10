// Fill out your copyright notice in the Description page of Project Settings.


#include "Grunt.h"
#include "Monster.h"	

AGrunt::AGrunt()
{
	MonsterName = FString(TEXT("Grunt"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GRUNT(TEXT("SkeletalMesh'/Game/_Game/Monster/MasterGrunt/Mesh/SK_Master_Grunt.SK_Master_Grunt'"));
	if (SK_GRUNT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GRUNT.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.f), FRotator(0.0f, -90.f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> GruntAnim(TEXT("AnimBlueprint'/Game/_Game/Monster/MasterGrunt/Animation/BP_MasterGruntAnim.BP_MasterGruntAnim_C'"));
	if (GruntAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GruntAnim.Class);
	}

	SetAttackDamage(7.f);
}