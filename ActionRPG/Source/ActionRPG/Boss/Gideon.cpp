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
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Sound/SoundCue.h"

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_METEORCAST(TEXT("ParticleSystem'/Game/_Game/Gideon/FX/P_Gideon_Meteor_Portal.P_Gideon_Meteor_Portal'"));
	if (PS_METEORCAST.Succeeded())
	{
		MeteorCastParitle = PS_METEORCAST.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_METEORFIRE(TEXT("ParticleSystem'/Game/_Game/Gideon/FX/P_Gideon_MeteorBigImpact_Undertow.P_Gideon_MeteorBigImpact_Undertow'"));
	if (PS_METEORFIRE.Succeeded())
	{
		MeteorFireParticle = PS_METEORFIRE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_METEOSHOWER(TEXT("ParticleSystem'/Game/_Game/Gideon/FX/P_Gideon_Meteor_Shower.P_Gideon_Meteor_Shower'"));
	if (PS_METEOSHOWER.Succeeded())
	{
		MeteorShowerParticle = PS_METEOSHOWER.Object;
	}

	FireAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FIREAURA"));
	FireAura->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FireAura(TEXT("ParticleSystem'/Game/_Game/Gideon/FX/P_Charged_AOE_Fire_00.P_Charged_AOE_Fire_00'"));
	if (PS_FireAura.Succeeded())
	{
		FireAura->SetTemplate(PS_FireAura.Object);
	}
	FireAura->bAutoActivate = false;

	FireSpin = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FIRESPIN"));
	FireSpin->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FireSpin(TEXT("ParticleSystem'/Game/_Game/Gideon/FX/P_Moving_Spin_Fire_00.P_Moving_Spin_Fire_00'"));
	if (PS_FireSpin.Succeeded())
	{
		FireSpin->SetTemplate(PS_FireSpin.Object);
	}

	FireSpin->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Fireball(TEXT("SoundCue'/Game/_Game/Assets/Sound/Fireball.Fireball'"));
	if (SC_Fireball.Succeeded())
	{
		FireballSound = SC_Fireball.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Meteor(TEXT("SoundCue'/Game/_Game/Assets/Sound/Meteor_Impact_Sound_Effect_Cue.Meteor_Impact_Sound_Effect_Cue'"));
	if (SC_Meteor.Succeeded())
	{
		MeteorSound = SC_Meteor.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_OpeningSound(TEXT("SoundCue'/Game/ParagonGideon/Audio/Wavs/GideonOpening.GideonOpening'"));
	if (SC_OpeningSound.Succeeded())
	{
		OpeningSound = SC_OpeningSound.Object;
	}


}

// Called when the game starts or when spawned
void AGideon::BeginPlay()
{
	Super::BeginPlay();
	SetHP(MaxHP);
	
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (MainCharacter)
	{
		MainCharacter->SetBoss(this);
	}
}

float AGideon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (false == bCanbeAttacked)
		return 0.f;
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
			GAnimInstance->OnMeteorCast.AddUObject(this, &AGideon::MeteorCast);
			GAnimInstance->OnFireMeteor.AddUObject(this, &AGideon::MeteorFire);
			GAnimInstance->OnTranformEnd.AddUObject(this, &AGideon::TransformEffect);
		}
	}

	MyAiController = Cast<AGideonAIController>(GetController());
}

bool AGideon::IsDead()
{
	return bIsDead;
}

void AGideon::MeteorCast()
{
	UE_LOG(LogTemp, Warning, TEXT("cast in gideon"));
	SetCanBeAttacked(false);

	auto GideonAIController = Cast<AGideonAIController>(GetController());
	if (GideonAIController)
	{
		MeteorTarget = Cast<AMainCharacter>(GideonAIController->GetBlackboardComponent()->GetValueAsObject(AGideonAIController::TargetKey));
		MeteorVector = MeteorTarget->GetActorLocation();
	}

	
}

void AGideon::MeteorFire()
{
	UE_LOG(LogTemp, Warning, TEXT("fire in gideon"));
	SetCanBeAttacked(true);

	if (bIsDead)
		return;

	if (true == MyAiController->GetBlackboardComponent()->GetValueAsBool(AGideonAIController::IsSecondPageIn))
	{
		MeteorVector = MainCharacter->GetActorLocation();
		GetWorldTimerManager().SetTimer(MeteorOneShotTimer, this, &AGideon::SendMeteorOneShot, MeteorOneShotTime);

		GetWorldTimerManager().SetTimer(SecondPageMeteorTimer, this, &AGideon::RepeatMeteor, MeteorTime);
	}
	else
	{

		MeteorVector = MainCharacter->GetActorLocation();
		GetWorldTimerManager().SetTimer(MeteorOneShotTimer, this, &AGideon::SendMeteorOneShot, MeteorOneShotTime);
	}

}

void AGideon::TransformEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("TRANFORMEFFECT"));
	FireAura->Activate();
	FireSpin->Activate();

	MyAiController->GetBlackboardComponent()->SetValueAsBool(AGideonAIController::IsSecondPageIn, true);
	SetCanBeAttacked(true);
	OnTransformEndGideon.Broadcast();
}

void AGideon::TransformPage2()
{
	if (GAnimInstance)
	{
		GAnimInstance->PlayTransformMontage();
	}
}

void AGideon::SetCanBeAttacked(bool CanBe)
{
	bCanbeAttacked = CanBe;
}

void AGideon::RepeatMeteor()
{
	MeteorSpawnVector = MainCharacter->GetActorLocation();
	GetWorldTimerManager().SetTimer(GetMeteorVectorTimer, this, &AGideon::SendMeteorRepeat, GetMeteorVectorRepeatTime);
}

void AGideon::SendMeteorRepeat()
{
	if (MeteorRepeatCount < 2)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), MeteorSound);
		MeteorRepeatCount++;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorCastParitle, MeteorVector + FVector(0.0f, 0.0f, 400.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorVector + FVector(0.0f, 0.0f, -80.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorSpawnVector + FVector(0.0f, 0.0f, -80.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorSpawnVector + FVector(300.0f, 0.0f, -80.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorSpawnVector + FVector(-300.0f, 0.0f, -80.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorSpawnVector + FVector(0.f, 300.f, -80.f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorSpawnVector + FVector(-0.f, 300.f, -80.f));

		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			MeteorSpawnVector,
			MeteorSpawnVector + FVector(0.0f, 0.0f, 100.f),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(500.f),
			Params);

		if (bResult)
		{
			if (HitResult.Actor.IsValid())
			{
				//UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR :%s"), *HitResult.Actor->GetName());

				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(20.f, DamageEvent, GetController(), this);
			}
		}

		MeteorSpawnVector = MainCharacter->GetActorLocation();
		GetWorldTimerManager().SetTimer(SecondPageMeteorTimer, this, &AGideon::SendMeteorRepeat, MeteorTime);
	}
	else
	{
		MeteorRepeatCount = 0;
	}
}

void AGideon::SendMeteorOneShot()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorCastParitle, MeteorVector + FVector(0.0f, 0.0f, 400.f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorFireParticle, MeteorVector + FVector(0.0f, 0.0f, -80.f));
	UGameplayStatics::PlaySound2D(GetWorld(), MeteorSound);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		MeteorVector,
		MeteorVector + FVector(0.0f, 0.0f, 100.f),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(500.f),
		Params);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(20.f, DamageEvent, GetController(), this);
		}
	}
}

void AGideon::PlayOpeningSound()
{
	if (OpeningSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OpeningSound);
	}
}

void AGideon::FireFireBall()
{
	UGideonAniminstance* Anim = Cast<UGideonAniminstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->PlayAttackMontage();
		Anim->JumpToSection(AttackSectionIndex);
		AttackSectionIndex = (AttackSectionIndex + 1) % 4;

		FVector TargetLocation = MyAiController->GetBlackboardComponent()->GetValueAsVector(AGideonAIController::TargetKey);
		AFireBall* Fireball = GetWorld()->SpawnActor<AFireBall>(GetActorLocation(), FRotator::ZeroRotator);
		Fireball->SetFireballRotation(GetActorRotation());
		Fireball->SetOwner(this);

		UGameplayStatics::PlaySound2D(GetWorld(), FireballSound);
	}
}

void AGideon::FireMeteor()
{
	if (GAnimInstance)
	{
		GAnimInstance->PlayMeteorMontage();
	}
}

void AGideon::FireFireBall2()
{
	UGideonAniminstance* Anim = Cast<UGideonAniminstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->PlayAttackMontage();
		Anim->JumpToSection(AttackSectionIndex);
		AttackSectionIndex = (AttackSectionIndex + 1) % 4;

		FVector TargetLocation = MyAiController->GetBlackboardComponent()->GetValueAsVector(AGideonAIController::TargetKey);
		UGameplayStatics::PlaySound2D(GetWorld(), FireballSound);

		for (int i = -1; i < 2; i++)
		{
			
			AFireBall* Fireball = GetWorld()->SpawnActor<AFireBall>(GetActorLocation(), FRotator::ZeroRotator);
			Fireball->SetFireballRotation(GetActorRotation() + FRotator(0.0f, -15.f * i , 0.0f));
			Fireball->SetOwner(this);
		}
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


	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(MainCharacter->GetController());
	if (MainPlayerController)
	{
		MainPlayerController->SetBossHPWidgetVisibility(false);
	}

	SetActorEnableCollision(false);
	GetMovementBase()->SetHiddenInGame(false);
	//HPBar->SetHiddenInGame(true);
	GAnimInstance->SetDeadAnim();
	SetCanBeDamaged(false);
	MyAiController->StopAI();
	LockOnParticle->SetVisibility(false);
	FireSpin->Deactivate();

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		Destroy();
		auto TargetController = Cast<AMainPlayerController>(MeteorTarget->GetController());
		if(TargetController)
		{
			TargetController->OnGameEnd();
		}
	}), 5.f, false);
}

void AGideon::SetHPBarVisiblity(bool Visibility)
{
	//HPBar->SetVisibility(Visibility);
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