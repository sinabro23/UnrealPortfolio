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
#include "GideonHealthbarWidget.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"

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

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	HPBar->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW_GHPBAR(TEXT("WidgetBlueprint'/Game/_Game/UI/MainHUD/GideonHealthbarWidget.GideonHealthbarWidget_C'"));
	if (UW_GHPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UW_GHPBAR.Class);
		HPBar->SetDrawSize(FVector2D(500.f, 40.f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> GideonAnim(TEXT("AnimBlueprint'/Game/_Game/Gideon/Animation/GideonAnim.GideonAnim_C'"));
	if (GideonAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GideonAnim.Class);
	}
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCharacter"));

	AIControllerClass = AGideonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	LockOnParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LOCKONPARTICLE"));
	LockOnParticle->SetupAttachment(GetMesh());
	LockOnParticle->SetRelativeRotation(FRotator(0.0f, 90.f, 0.0f));
	LockOnParticle->bAutoActivate = true;
	LockOnParticle->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_GLOCKON(TEXT("ParticleSystem'/Game/_Game/FX/P_Targeting_Player_Select.P_Targeting_Player_Select'"));
	if (PS_GLOCKON.Succeeded())
	{
		LockOnParticle->SetTemplate(PS_GLOCKON.Object);
	}
}

// Called when the game starts or when spawned
void AGideon::BeginPlay()
{
	Super::BeginPlay();
	SetHP(MaxHP);
}

float AGideon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SetHP(CurrentHP - DamageAmount);

	return DamageAmount;
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

	HPBar->InitWidget();
	auto HPWidget = Cast<UGideonHealthbarWidget>(HPBar->GetUserWidgetObject());
	if (HPWidget)
	{
		HPWidget->BindHP(this);
	}
}

bool AGideon::IsDead()
{
	return bIsDead;
}

void AGideon::FireFireBall()
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
		Fireball->SetOwner(this);
	}
}

void AGideon::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
}

void AGideon::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	if (CurrentHP < 0)
	{
		CurrentHP = 0;
		Death();
	}

	OnHPChanged.Broadcast();
}

float AGideon::GetHPRatio()
{
	return CurrentHP / MaxHP;
}

void AGideon::Death()
{
	bIsDead = true;
	SetActorEnableCollision(false);
	GetMovementBase()->SetHiddenInGame(false);
	HPBar->SetHiddenInGame(true);
	GAnimInstance->SetDeadAnim();
	SetCanBeDamaged(false);
	GideonAIController->StopAI();

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		Destroy();
	}), 5.f, false);
}

void AGideon::SetHPBarVisiblity(bool Visibility)
{
	HPBar->SetVisibility(Visibility);
}

void AGideon::LockOn()
{
	if (!bLockOnVisible)
	{
		LockOnParticle->SetVisibility(true);
		bLockOnVisible = true;
	}

}

void AGideon::LockOff()
{
	if (bLockOnVisible)
	{
		LockOnParticle->SetVisibility(false);
		bLockOnVisible = false;
	}

}