// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterAIController.h"
#include "MonsterHealthbarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "MonsterNameWidget.h"
#include "DrawDebugHelpers.h"
// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, HPBarHeight));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW_HPBAR(TEXT("WidgetBlueprint'/Game/_Game/UI/Monster/MonsterHealthBar.MonsterHealthBar_C'"));
	if (UW_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UW_HPBAR.Class);
		HPBar->SetDrawSize(FVector2D(300.f, 50.f));
	}

	NameBox = CreateDefaultSubobject<UWidgetComponent>(TEXT("NAMEBOX"));
	NameBox->SetupAttachment(HPBar);
	NameBox->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW_NAMEBOX(TEXT("WidgetBlueprint'/Game/_Game/UI/Monster/MonsterName.MonsterName_C'"));
	if (UW_NAMEBOX.Succeeded())
	{
		NameBox->SetWidgetClass(UW_NAMEBOX.Class);
		NameBox->SetDrawSize(FVector2D(300.f, 20.f));
	}
			

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCharacter"));

	ExclamationMark = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EXCLAMATION"));
	ExclamationMark->SetupAttachment(GetRootComponent());
	ExclamationMark->SetRelativeLocation(FVector(0.0f, 0.0f, ExclamationMarkHeight));
	ExclamationMark->bAutoActivate = true;
	ExclamationMark->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_EXCLAMATION(TEXT("ParticleSystem'/Game/_Game/FX/P_Status_Quest.P_Status_Quest'"));
	if (PS_EXCLAMATION.Succeeded())
	{
		ExclamationMark->SetTemplate(PS_EXCLAMATION.Object);
	}

	BloodParitcle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BLOOD"));
	BloodParitcle->SetupAttachment(GetRootComponent());
	BloodParitcle->bAutoActivate = false;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_BLOOD(TEXT("ParticleSystem'/Game/_Game/FX/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if (PS_BLOOD.Succeeded())
	{
		BloodParitcle->SetTemplate(PS_BLOOD.Object);
	}

	LockOnParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LOCKONPARTICLE"));
	LockOnParticle->SetupAttachment(GetMesh());
	LockOnParticle->bAutoActivate = true;
	LockOnParticle->SetVisibility(false);


	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_LOCKON(TEXT("ParticleSystem'/Game/_Game/FX/P_Targeting_Player_Select.P_Targeting_Player_Select'"));
	if (PS_LOCKON.Succeeded())
	{
		LockOnParticle->SetTemplate(PS_LOCKON.Object);
	}

	MonsterName = FString(TEXT("NONE"));

	AttackRange = 200.f;
	AttackRadius = 50.f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	MonsterAIController = Cast<AMonsterAIController>(GetController());
	if (MonsterAIController)
	{
		MonsterAIController->RunAI();
	}



}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HPBar->InitWidget();
	auto HPWidget = Cast<UMonsterHealthbarWidget>(HPBar->GetUserWidgetObject());
	if (HPWidget)
	{
		HPWidget->BindHP(this);
	}

	NameBox->InitWidget();
	auto NameBoxWidget = Cast<UMonsterNameWidget>(NameBox->GetUserWidgetObject());
	if (NameBoxWidget)
	{
		NameBoxWidget->BindName(this);
	}


	auto Anim = GetMesh()->GetAnimInstance();
	if (Anim)
	{
		MonsterAnim = Cast<UMonsterAnimInstance>(Anim);
		if (MonsterAnim)
		{
			MonsterAnim->OnMontageEnded.AddDynamic(this, &AMonster::OnAttackMontageEnded);
			MonsterAnim->OnAttackHitCheck.AddUObject(this, &AMonster::AttackHitCheck);
		}
	}

	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, HPBarHeight));

	LockOnParticle->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//UE_LOG(LogTemp, Warning, TEXT("Actor %s took Damage %f"), *GetName(), FinalDamage);
	float TempHP = CurrentHP - DamageAmount;
	SetHP(TempHP);

	if (BloodParitcle)
	{
		BloodParitcle->Activate(true);
	}
	
	return FinalDamage;
}

void AMonster::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	if (CurrentHP < 0)
	{
		CurrentHP = 0;
		Dead();
	}
	

	OnHPChanged.Broadcast();
}

float AMonster::GetHPRatio()
{
	return CurrentHP / MaxHP;
}



void AMonster::Dead()
{
	SetActorEnableCollision(false);
	GetMovementBase()->SetHiddenInGame(false);
	HPBar->SetHiddenInGame(true);
	NameBox->SetHiddenInGame(true);
	MonsterAnim->SetDeadAnim();
	SetCanBeDamaged(false);
	MissEnemy();
	MonsterAIController->StopAI();

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		Destroy();
	}), 5.f, false);
}

FString AMonster::GetMonsterName()
{
	return MonsterName;
}


void AMonster::Attack()
{
	if (bIsAttacking)
		return;

	if (MonsterAnim)
	{
		MonsterAnim->PlayAttackMontage();
		MonsterAnim->JumpToSection(AttackSectionIndex);
		AttackSectionIndex = (AttackSectionIndex + 1) % 4;
	}

	bIsAttacking = true;
}

void AMonster::AttackHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);



	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;  // ?
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat(); // ?
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.f;
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);


	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			//UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR :%s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}
}

void AMonster::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AMonster::LockOn()
{
	LockOnParticle->SetVisibility(true);
}

void AMonster::LockOff()
{
	LockOnParticle->SetVisibility(false);
}

void AMonster::FindEnemy()
{
	ExclamationMark->SetVisibility(true);
}

void AMonster::MissEnemy()
{
	ExclamationMark->SetVisibility(false);
}