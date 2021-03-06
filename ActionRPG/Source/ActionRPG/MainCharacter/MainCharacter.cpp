// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterAnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Monster.h"
#include "TimerManager.h"
#include "MainPlayerController.h"
#include "Components/SphereComponent.h"
#include "BlessingStatue.h"
#include "MainSaveGame.h"
#include "HPPotion.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MainPlayerController.h"
#include "Gideon.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_MESH(TEXT("SkeletalMesh'/Game/_Game/Character/Mesh/KwangRosewood.KwangRosewood'"));
	if (SM_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_MESH.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MainCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = DeafaultSpinrgArmLength;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.f));
	SpringArm->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = 650.f;


	// 애님인스턴스관련
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> MAINANIM(TEXT("AnimBlueprint'/Game/_Game/Character/Animation/BP_MainCharacterAnim.BP_MainCharacterAnim_C'"));
	if (MAINANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MAINANIM.Class);
	}

	// 컨트롤러 따라 메쉬 안돌게만들기
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 컨트롤러 방향에따라 캐릭터 돌게 만들기
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_ATTACK(TEXT("SoundCue'/Game/_Game/Character/Sound/Kwang_Effort_Attack.Kwang_Effort_Attack'"));
	if (SC_ATTACK.Succeeded())
	{
		AttackSound = SC_ATTACK.Object;
	}

	SphereForTargetHPBar = CreateDefaultSubobject<USphereComponent>(TEXT("SPHEREFORTARGET"));
	SphereForTargetHPBar->SetupAttachment(GetRootComponent());
	SphereForTargetHPBar->InitSphereRadius(1000.f);
	SphereForTargetHPBar->SetHiddenInGame(true);

	NearRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NEARRANGESPHERE"));
	NearRangeSphere->SetupAttachment(GetRootComponent());
	NearRangeSphere->InitSphereRadius(300.f);
	NearRangeSphere->SetHiddenInGame(true);

	AttackRange = 200.f;
	AttackRadius = 50.f;

	LockOnLookAtRotation = FRotator(0.0f);

	RMBSkillParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RMBSKILLPARTICLE"));
	RMBSkillParticle->SetupAttachment(GetRootComponent());
	RMBSkillParticle->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_RMB(TEXT("ParticleSystem'/Game/_Game/FX/P_Kwang_LightStrike_Burst.P_Kwang_LightStrike_Burst'"));
	if (PS_RMB.Succeeded())
	{
		RMBSkillParticle->SetTemplate(PS_RMB.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_RMBFX(TEXT("ParticleSystem'/Game/_Game/FX/P_Kwang_LightStrike_Burst_Holding.P_Kwang_LightStrike_Burst_Holding'"));
	if (PS_RMBFX.Succeeded())
	{
		RMBSkillFX = PS_RMBFX.Object;
	}

	BuffParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BUFFPARTICLE"));
	BuffParticle->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_BUFF(TEXT("ParticleSystem'/Game/_Game/FX/P_Status_LevelUp.P_Status_LevelUp'"));
	if (PS_BUFF.Succeeded())
	{
		BuffParticle->SetTemplate(PS_BUFF.Object);
	}
	BuffParticle->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Die(TEXT("SoundCue'/Game/_Game/Character/Sound/Kwang_Death.Kwang_Death'"));
	if (SC_ATTACK.Succeeded())
	{
		DieSound = SC_Die.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_RMB(TEXT("SoundCue'/Game/_Game/Assets/Sound/thunder-loop-1_Cue.thunder-loop-1_Cue'"));
	if (SC_RMB.Succeeded())
	{
		RMBSound = SC_RMB.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Q(TEXT("SoundCue'/Game/_Game/Character/Sound/Kwang_Effort_Ability_Q.Kwang_Effort_Ability_Q'"));
	if (SC_Q.Succeeded())
	{
		QSound = SC_Q.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_R(TEXT("SoundCue'/Game/_Game/Assets/Sound/cable-swish-whoosh-through-air-3-sound-effect-95306397_Cue.cable-swish-whoosh-through-air-3-sound-effect-95306397_Cue'"));
	if (SC_R.Succeeded())
	{
		RSkillSound = SC_R.Object;
	}

	HealingParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HealingParticle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Healing(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Priest_Heal_Over_Time_3D.P_Priest_Heal_Over_Time_3D'"));
	if (PS_Healing.Succeeded())
	{
		HealingParticle->SetTemplate(PS_Healing.Object);
	}

	HealingParticle->SetupAttachment(GetRootComponent());
	HealingParticle->SetActive(true);

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_Item(TEXT("SoundCue'/Game/Interface_And_Item_Sounds/Cues/Click_06_Cue.Click_06_Cue'"));
	if (SC_Item.Succeeded())
	{
		TakeItemSound = SC_Item.Object;
	}
}

void AMainCharacter::SetMovementStatus(EMovementStatus NewStatus)
{
	MovementStatus = NewStatus;

	switch (MovementStatus)
	{
	case EMovementStatus::EMS_NORMAL:
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		break;
	case EMovementStatus::EMS_SPRINTING:
		GetCharacterMovement()->MaxWalkSpeed = SpringtingSpeed;	
		break;
	case EMovementStatus::EMS_MAX:
		break;
	default:
		break;
	}
}



void AMainCharacter::CapslockKeyDown()
{
	if (MainAnim)
	{
		MainAnim->SetSprintingAnim();
		switch (MovementStatus)
		{
		case EMovementStatus::EMS_NORMAL:
			SetMovementStatus(EMovementStatus::EMS_SPRINTING);
			break;
		case EMovementStatus::EMS_SPRINTING:
			SetMovementStatus(EMovementStatus::EMS_NORMAL);
			break;
		case EMovementStatus::EMS_MAX:
			break;
		default:
			break;
		}
	}

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	LockOnLookAtRotation = FRotator(GetActorRotation());
	HealingParticle->SetActive(true);
	HealingParticle->SetHiddenInGame(true);
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MainAnim = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (MainAnim)
	{
		MainAnim->OnMontageEnded.AddDynamic(this, &AMainCharacter::OnAttackMontageEnded);
		MainAnim->OnAttackHitCheck.AddUObject(this, &AMainCharacter::AttackHitCheck);
	}

	SphereForTargetHPBar->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnSphereOverlappedForHPBar);
	SphereForTargetHPBar->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnSphereEndOverlappedForHPBar);

	NearRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnSphereOverlappedForNear);
	NearRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnSphereEndOverlappedForNear);

	FName WeaponSocket(TEXT("weapon_particle"));
	RMBSkillParticle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsImmortal || !CharacterCanBeDamaged)
		return 0;

	SetHP(CurrentHP - DamageAmount);

	return DamageAmount;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLockOn)
	{
		if (CurrentTargetMonster.IsValid() && !CurrentTargetMonster->IsDead())
		{
			float Distance = (GetActorLocation() - CurrentTargetMonster->GetActorLocation()).Size();
			if (Distance >= 1000.f)
			{
				bIsLockOn = false;
				if (CurrentTargetMonster.IsValid())
				{
					CurrentTargetMonster->LockOff();
					CurrentTargetMonster = nullptr;
				}
				return;
			}

			FRotator LookAtRotation = GetLookAtRotationYaw(CurrentTargetMonster->GetActorLocation());
			LockOnLookAtRotation = FRotator(GetControlRotation().Pitch, LookAtRotation.Yaw, 0.f);

			GetController()->SetControlRotation(LockOnLookAtRotation);
		}
		else
		{
			bIsLockOn = false;
			CurrentTargetMonster = nullptr;
		}
			
	}

	if (bIsLockOnBoss)
	{
		if (CurrentTargetGideon.IsValid() && !CurrentTargetGideon->IsDead())
		{
			float Distance = (GetActorLocation() - CurrentTargetGideon->GetActorLocation()).Size();
			if (Distance >= 1000.f)
			{
				bIsLockOnBoss = false;
				if (CurrentTargetGideon.IsValid())
				{
					CurrentTargetGideon->LockOff();
					CurrentTargetGideon = nullptr;
				}
				return;
			}

			FRotator LookAtRotation = GetLookAtRotationYaw(CurrentTargetGideon->GetActorLocation());
			LockOnLookAtRotation = FRotator(GetControlRotation().Pitch, LookAtRotation.Yaw, 0.f);

			GetController()->SetControlRotation(LockOnLookAtRotation);
		}
		else
		{
			bIsLockOnBoss = false;
			CurrentTargetGideon = nullptr;
		}
	}

	float DeltaStamina = StaminaDrainRate * DeltaTime;
	if (MovementStatus == EMovementStatus::EMS_SPRINTING)
	{
		CurrentStamina -= DeltaStamina;
		
		if (CurrentStamina <= 0.f)
		{
			MainAnim->SetSprintingAnim();
			CurrentStamina = 0.f;
			SetMovementStatus(EMovementStatus::EMS_NORMAL);
		}

		if (GetCharacterMovement()->IsFalling() || 0.f == GetVelocity().Size())
		{
			MainAnim->SetSprintingAnim();
			SetMovementStatus(EMovementStatus::EMS_NORMAL);
		}
	}
	else
	{
		if (CurrentStamina <= MaxStamina)
		{
			CurrentStamina += DeltaStamina;
			if (CurrentStamina > MaxStamina)
			{
				CurrentStamina = MaxStamina;
			}
		}
	}

	if(IsDrinkingHPPotion)
	{
		if (CurrentHP <= MaxHP)
		{
			SetHP(CurrentHP + DeltaTime * 20.f);
		}
		
	}

	if(IsDrinkingMPPotion)
	{
		if (CurrentMP <= MaxMP)
		{
			SetMP(CurrentMP + DeltaTime * 20.f);
		}
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shift"), EInputEvent::IE_Pressed, this, &AMainCharacter::ShiftKey);
	PlayerInputComponent->BindAction(TEXT("Tap"), EInputEvent::IE_Pressed, this, &AMainCharacter::LockOn);
	PlayerInputComponent->BindAction(TEXT("RMB"), EInputEvent::IE_Pressed, this, &AMainCharacter::RMBSkill);

	PlayerInputComponent->BindAction(TEXT("EKey"), EInputEvent::IE_Pressed, this, &AMainCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(TEXT("EKey"), EInputEvent::IE_Released, this, &AMainCharacter::EKeyReleased);

	PlayerInputComponent->BindAction(TEXT("RKey"), EInputEvent::IE_Pressed, this, &AMainCharacter::RKeyPressed);

	PlayerInputComponent->BindAction(TEXT("TestKey1"), EInputEvent::IE_Pressed, this, &AMainCharacter::SaveGame);
	PlayerInputComponent->BindAction(TEXT("TestKey2"), EInputEvent::IE_Pressed, this, &AMainCharacter::LoadGame);

	PlayerInputComponent->BindAction(TEXT("HPPotion"), EInputEvent::IE_Pressed, this, &AMainCharacter::HPPotion);
	PlayerInputComponent->BindAction(TEXT("MPPotion"), EInputEvent::IE_Pressed, this, &AMainCharacter::MPPotion);
	PlayerInputComponent->BindAction(TEXT("F12Key"), EInputEvent::IE_Pressed, this, &AMainCharacter::PKeyPressed);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMainCharacter::CapslockKeyDown);
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMainCharacter::CapslockKeyUp);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMainCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), this, &AMainCharacter::CameraZoom);
}

void AMainCharacter::AddControllerYawInput(float Val)
{
	if (bIsLockOn || bIsLockOnBoss)
		return;
	Super::AddControllerYawInput(Val);
}

void AMainCharacter::Jump()
{
	if (bIsDead)
		return;
	Super::Jump();
}

bool AMainCharacter::IsCharacterInBossRoom()
{
	return bIsInBossRoom;
}

void AMainCharacter::SetCharacterInBossRoom(bool IsIn)
{
	bIsInBossRoom = IsIn;
}

void AMainCharacter::UpDown(float InputValue)
{
	if (bIsAttacking || bIsDead)
		return;

	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, InputValue);
}

void AMainCharacter::LeftRight(float InputValue)
{
	if (bIsAttacking || bIsDead)
		return;

	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, InputValue);
}

void AMainCharacter::CameraZoom(float InputValue)
{
	if (0 == InputValue)
		return;

	const float NewTargetArmLength = SpringArm->TargetArmLength + InputValue * ZoomStep;
	SpringArm->TargetArmLength = FMath::Clamp(NewTargetArmLength, MinSpringArmLength, MaxSpringArmLength);
}

void AMainCharacter::Attack()
{
	if (bIsAttacking || bIsDead)
		return;

	if (MainAnim)
	{
		if (bIsLockOn || bIsLockOnBoss)
		{
			SetActorRotation(FRotator(0.f, LockOnLookAtRotation.Yaw, 0.f));
		}
		UGameplayStatics::PlaySound2D(GetWorld(), AttackSound);

		MainAnim->PlayAttackMontage();
		MainAnim->JumpToSection(AttackSectionIndex);
		AttackSectionIndex = (AttackSectionIndex + 1) % 4;
	}

	bIsAttacking = true;

}

void AMainCharacter::AttackHitCheck()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;  // ?
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat(); // ?
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.f;
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.Actor.IsValid())
			{
			UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR :%s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			}
		}
	}
}

void AMainCharacter::ShiftKey()
{
	if (bIsAttacking || GetMovementComponent()->IsFalling())
		return;


	if (MainAnim && CurrentStamina >= 40.f)
	{
		MainAnim->PlayEvadeMontage();
		CharacterCanBeDamaged = false;
		bIsAttacking = true;
		CurrentStamina -= 40.f;
	}
}

void AMainCharacter::EKeyPressed()
{
	IsEkeyPressed = true;
	if (bIsNearSphereOverlapped)
	{
		Blessed();
	}

	if (bCanBuy)
	{
		AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->OnPotionShop();
		}
	}
}

void AMainCharacter::EKeyReleased()
{
	IsEkeyPressed = false;
}

void AMainCharacter::RKeyPressed()
{
	if (bIsAttacking || CurrentMP <= 5.f)
		return;
	MainAnim->RSkillMontagePlay();
	SetMP(CurrentMP - 5.f);

	UGameplayStatics::PlaySound2D(GetWorld(), RSkillSound);

	FVector Center = GetActorLocation();
	TArray<FOverlapResult> HitResults;
	FCollisionQueryParams CollsionQueryParam(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(200.f),
		CollsionQueryParam
	);


	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.Actor.IsValid())
			{

				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(35.f, DamageEvent, GetController(), this);
			}
		}
	}


	bIsAttacking = true;
}




void AMainCharacter::LockOn()
{
	if (bIsLockOn)
	{
		if (CurrentTargetMonster.IsValid())
		{
			CurrentTargetMonster->LockOff();
		}
		bIsLockOn = false;
	}
	else
	{
		if (bIsDead)
			return;

		FVector Center = GetActorLocation() + Camera->GetForwardVector() * 500.f;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollsionQueryParam(NAME_None, false, this);
		bool bResult = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel4,
			FCollisionShape::MakeSphere(LockOnRange),
			CollsionQueryParam
		);

		if (bResult)
		{
			for (auto const& OverlapResult : OverlapResults)
			{
				auto Monster = Cast<AMonster>(OverlapResult.GetActor());
				if (Monster)
				{
					//DrawDebugSphere(GetWorld(), Center, LockOnRange, 16, FColor::Green, false, 0.5f);
					//DrawDebugPoint(GetWorld(), Monster->GetActorLocation(), 10.f, FColor::Blue, false, 0.5f);
					//DrawDebugLine(GetWorld(), Center, Monster->GetActorLocation(), FColor::Blue, false, 0.5f);
					bIsLockOn = true;

					float MonsterDistance = (Monster->GetActorLocation() - GetActorLocation()).Size();
					if (CurrentTargetMonster.IsValid())
					{
						float CurrentTargetDistance = (CurrentTargetMonster->GetActorLocation() - GetActorLocation()).Size();
						if (MonsterDistance < CurrentTargetDistance)
						{
							CurrentTargetMonster = Monster;
						}
					}
					else
					{
						CurrentTargetMonster = Monster;
					}
				}
			}

			if (CurrentTargetMonster.IsValid())
			{
				CurrentTargetMonster->LockOn();
			}
		}
	}

	if (bIsLockOnBoss)
	{
		if (CurrentTargetGideon.IsValid())
		{
			CurrentTargetGideon->LockOff();
		}
		bIsLockOnBoss = false;
	}
	else
	{
		if (bIsDead)
			return;

		FVector Center = GetActorLocation() + Camera->GetForwardVector() * 500.f;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollsionQueryParam(NAME_None, false, this);
		bool bResult = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel4,
			FCollisionShape::MakeSphere(LockOnRange),
			CollsionQueryParam
		);

		if (bResult)
		{
			for (auto const& OverlapResult : OverlapResults)
			{
				auto Gideon = Cast<AGideon>(OverlapResult.GetActor());
				if (Gideon)
				{
					bIsLockOnBoss = true;

					float GideonDistance = (Gideon->GetActorLocation() - GetActorLocation()).Size();
					if (CurrentTargetGideon.IsValid())
					{
						float CurrentTargetDistance = (CurrentTargetGideon->GetActorLocation() - GetActorLocation()).Size();
						if (GideonDistance < CurrentTargetDistance)
						{
							CurrentTargetGideon = Gideon;
						}
					}
					else
					{
						CurrentTargetGideon = Gideon;
					}
				}
			}

			if (CurrentTargetGideon.IsValid())
			{
				CurrentTargetGideon->LockOn();
			}
		}
	}
}

void AMainCharacter::Blessed()
{
	BuffParticle->Activate(true);
	SetHP(MaxHP);
	SetMP(MaxMP);
	CurrentStamina = MaxStamina;
}

void AMainCharacter::RMBSkill()
{
	if (bIsAttacking || CurrentMP <= 15.f)
		return;
	UE_LOG(LogTemp, Warning, TEXT("RMB"));
	MainAnim->RMBSkillMontagePlay();
	SetMP(CurrentMP - 15.f);

	FVector Center = GetActorLocation();
	TArray<FOverlapResult> HitResults;
	FCollisionQueryParams CollsionQueryParam(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(RMBSkillRange),
		CollsionQueryParam
	);

	RMBSkillParticle->Activate(true);
	UGameplayStatics::PlaySound2D(GetWorld(), RMBSound);

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.Actor.IsValid())
			{
				//UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR :%s"), *HitResult.Actor->GetName());

				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(RMBSkillDamage, DamageEvent, GetController(), this);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RMBSkillFX, HitResult.Actor->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}

	//DrawDebugSphere(GetWorld(), Center, RMBSkillRange, 16, FColor::Green, false, 1.f);

	bIsAttacking = true;
}

void AMainCharacter::Dead()
{
	if (MainAnim)
	{
		MainAnim->SetDeadAnim();
	}

	bIsDead = true;
	SetActorEnableCollision(false);
	SetCanBeDamaged(false);

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

		auto PlayerController = Cast<AMainPlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->RestartLevel();
		}

	}), 10.f, false);
}

void AMainCharacter::HPPotion()
{
	if (HPPotionCount <= 0)
	{
		HPPotionCount = 0;
		return;
	}
	else
	{
		HealingParticle->SetActive(true, true);
		HealingParticle->SetHiddenInGame(false);
		HPPotionCount--;
		IsDrinkingHPPotion = true;
		GetWorldTimerManager().SetTimer(HPPotionTimer, this, &AMainCharacter::ResetHPPotion, 1.5f);
	}

}

void AMainCharacter::MPPotion()
{

	if (MPPotionCount <= 0)
	{
		MPPotionCount = 0;
		return;
	}
	else
	{
		HealingParticle->SetActive(true, true);
		HealingParticle->SetHiddenInGame(false);
		IsDrinkingMPPotion = true;
		MPPotionCount--;
		GetWorldTimerManager().SetTimer(MPPotionTimer, this, &AMainCharacter::ResetMPPotion, 1.5f);
	}
}

void AMainCharacter::GetHPPotion()
{
	HPPotionCount += 1;
	UGameplayStatics::PlaySound2D(GetWorld(), TakeItemSound);
}

void AMainCharacter::GetMPPotion()
{
	MPPotionCount += 1;
	UGameplayStatics::PlaySound2D(GetWorld(), TakeItemSound);
}

void AMainCharacter::SetCanBuy(bool Input)
{
	bCanBuy = Input;
}

void AMainCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!CharacterCanBeDamaged)
	{
		CharacterCanBeDamaged = true;
	}
	bIsAttacking = false;
}

FRotator AMainCharacter::GetLookAtRotationYaw(FVector TargetVector)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetVector);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMainCharacter::OnSphereOverlappedForHPBar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Monster = Cast<AMonster>(OtherActor);
	if (Monster)
	{
		Monster->WidgetTurnOn();
	}
}

void AMainCharacter::OnSphereEndOverlappedForHPBar(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Monster = Cast<AMonster>(OtherActor);
	if (Monster)
	{
		Monster->WidgetTurnOff();
	}
}

void AMainCharacter::OnSphereOverlappedForNear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Statue = Cast<ABlessingStatue>(OtherActor);
	if(Statue)
	{
		Statue->TurnOnWidget();
		bIsNearSphereOverlapped = true;
		return;
	}
}

void AMainCharacter::OnSphereEndOverlappedForNear(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Statue = Cast<ABlessingStatue>(OtherActor);
	if (Statue)
	{
		Statue->TurnOffWidget();
		bIsNearSphereOverlapped = false;
	}
}

void AMainCharacter::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.f;
		Dead();
	}

	if (CurrentHP >= MaxHP)
	{
		CurrentHP = MaxHP;
	}
}

void AMainCharacter::SetMP(float NewMP)
{
	CurrentMP = NewMP;

	if (CurrentMP <= 0.f)
	{
		CurrentMP = 0.f;
	}

	if (CurrentMP >= MaxMP)
	{
		CurrentMP = MaxMP;
	}
}

void AMainCharacter::GetCoins(int32 NewCoin)
{
	Coins += NewCoin;
	UGameplayStatics::PlaySound2D(GetWorld(), TakeItemSound);
}

int32 AMainCharacter::GetCoinCounts()
{
	return Coins;
}

void AMainCharacter::SetCoins(int32 NewCoin)
{
	Coins = NewCoin;
}

float AMainCharacter::GetCurrentHP()
{
	return CurrentHP;
}

void AMainCharacter::ResetHPPotion()
{
	IsDrinkingHPPotion = false;
	HealingParticle->SetHiddenInGame(true);

}

void AMainCharacter::ResetMPPotion()
{
	IsDrinkingMPPotion = false;
	HealingParticle->SetHiddenInGame(true);
}

void AMainCharacter::PKeyPressed()
{
	if (IsImmortal)
	{
		IsImmortal = false;
		CharacterCanBeDamaged = false;
	}
	else
	{
		IsImmortal = true;
		CharacterCanBeDamaged = true;
	}
}

void AMainCharacter::SaveGame()
{
	UMainSaveGame* SaveGameInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.CurrentHP = CurrentHP;
	SaveGameInstance->CharacterStats.MaxHP = MaxHP;
	SaveGameInstance->CharacterStats.CurrentMP = CurrentMP;
	SaveGameInstance->CharacterStats.MaxMP = MaxMP;
	SaveGameInstance->CharacterStats.CurrentStamina = CurrentStamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void AMainCharacter::LoadGame()
{
	UMainSaveGame* LoadGameInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	LoadGameInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	CurrentHP = LoadGameInstance->CharacterStats.CurrentHP;
	MaxHP = LoadGameInstance->CharacterStats.MaxHP;
	CurrentMP = LoadGameInstance->CharacterStats.CurrentMP;
	MaxMP = LoadGameInstance->CharacterStats.MaxMP;
	CurrentStamina = LoadGameInstance->CharacterStats.CurrentStamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;

	SetActorLocation(LoadGameInstance->CharacterStats.Location);
	SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
}

void AMainCharacter::SetBoss(AGideon* Gideon)
{
	Boss = Gideon;
}

AGideon* AMainCharacter::GetBoss()
{
	return Boss;
}

