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

	GetCharacterMovement()->MaxWalkSpeed = 600.f;


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

	AttackRange = 200.f;
	AttackRadius = 50.f;

	LockOnLookAtRotation = FRotator(0.0f);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	LockOnLookAtRotation = FRotator(GetActorRotation());
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



}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!CharacterCanBeDamaged)
		return 0;

	CurrentHP -= DamageAmount;
	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.0f;
		Dead();
	}

	return DamageAmount;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLockOn)
	{
		if (CurrentTargetMonster.IsValid())
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
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shift"), EInputEvent::IE_Pressed, this, &AMainCharacter::ShiftKey);
	PlayerInputComponent->BindAction(TEXT("Tap"), EInputEvent::IE_Pressed, this, &AMainCharacter::LockOn);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMainCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), this, &AMainCharacter::CameraZoom);
}

void AMainCharacter::AddControllerYawInput(float Val)
{
	if (bIsLockOn)
		return;
	Super::AddControllerYawInput(Val);
}

void AMainCharacter::Jump()
{
	if (bIsDead)
		return;
	Super::Jump();
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
		if (bIsLockOn)
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
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#pragma region SingleAttack
	//FHitResult HitResult;
	//FCollisionQueryParams Params(NAME_None, false, this);

	//bool bResult = GetWorld()->SweepSingleByChannel(
	//	HitResult,
	//	GetActorLocation(),
	//	GetActorLocation() + GetActorForwardVector() * AttackRange,
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel2,
	//	FCollisionShape::MakeSphere(AttackRadius),
	//	Params);

	//FVector TraceVec = GetActorForwardVector() * AttackRange;
	//FVector Center = GetActorLocation() + TraceVec * 0.5f;
	//float HalfHeight = AttackRange * 0.5f + AttackRadius;  // ?
	//FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat(); // ?
	//FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//float DebugLifeTime = 5.f;
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

	//if (bResult)
	//{

	//	if (HitResult.Actor.IsValid())
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR :%s"), *HitResult.Actor->GetName());

	//		FDamageEvent DamageEvent;
	//		HitResult.Actor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	//	}
	//}

#pragma endregion

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			if (HitResult.Actor.IsValid())
			{
			//UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR :%s"), *HitResult.Actor->GetName());

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


	if (MainAnim)
	{
		MainAnim->PlayEvadeMontage();
		CharacterCanBeDamaged = false;
		bIsAttacking = true;
	}
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
					DrawDebugSphere(GetWorld(), Center, LockOnRange, 16, FColor::Green, false, 0.5f);
					DrawDebugPoint(GetWorld(), Monster->GetActorLocation(), 10.f, FColor::Blue, false, 0.5f);
					DrawDebugLine(GetWorld(), Center, Monster->GetActorLocation(), FColor::Blue, false, 0.5f);
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
		
			return;
		}

		DrawDebugSphere(GetWorld(), Center, LockOnRange, 16, FColor::Red, false, 0.5f);
	}
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

