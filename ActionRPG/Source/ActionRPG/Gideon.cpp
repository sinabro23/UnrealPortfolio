// Fill out your copyright notice in the Description page of Project Settings.


#include "Gideon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GideonAIController.h"
#include "MonsterHealthbarWidget.h"
#include "Components/WidgetComponent.h"
#include "FireBall.h"
#include "GideonAniminstance.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AGideon::AGideon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GIDEON(TEXT("SkeletalMesh'/Game/_Game/Gideon/Gideon_Mephisto.Gideon_Mephisto'"));
	if (SK_GIDEON.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GIDEON.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	static ConstructorHelpers::FClassFinder<UAnimInstance> GideonAnim(TEXT("AnimBlueprint'/Game/_Game/Gideon/Animation/GideonAnim.GideonAnim_C'"));
	if (GideonAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GideonAnim.Class);
	}
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCharacter"));

	AIControllerClass = AGideonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AGideon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGideon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGideon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGideon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto Anim = GetMesh()->GetAnimInstance();
	if (Anim)
	{
		GAnimInstance = Cast<UGideonAniminstance>(Anim);
		if (GAnimInstance)
		{
			GAnimInstance->OnMontageEnded.AddDynamic(this, &AGideon::OnAttackMontageEnded);
		}
	}

	GideonAIController = Cast<AGideonAIController>(GetController());
}

void AGideon::FireBall()
{
	UGideonAniminstance* Anim = Cast<UGideonAniminstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->PlayAttackMontage();
		Anim->JumpToSection(AttackSectionIndex);
		AttackSectionIndex = (AttackSectionIndex + 1) % 4;

		FVector TargetLocation = GideonAIController->GetBlackboardComponent()->GetValueAsVector(AGideonAIController::TargetKey);
		AFireBall* Fireball = GetWorld()->SpawnActor<AFireBall>(GetActorLocation(), FRotator::ZeroRotator);
		Fireball->SetFireballRotation(GetActorRotation());
	}
}

void AGideon::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
}

