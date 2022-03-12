// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGidon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABossGidon::ABossGidon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GIDONMESH(TEXT("SkeletalMesh'/Game/_Game/Gideon/Gideon_Mephisto.Gideon_Mephisto'"));
	if (SK_GIDONMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GIDONMESH.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> GidonAnim(TEXT("AnimBlueprint'/Game/_Game/Gideon/Animation/GideonAnim.GideonAnim_C'"));
	if (GidonAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GidonAnim.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCharacter"));

}

// Called when the game starts or when spawned
void ABossGidon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossGidon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossGidon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABossGidon::IsDead()
{
	return bIsDead;
}

