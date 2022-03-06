// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterAnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


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

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 500.f;
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
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MainAnim = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (MainAnim)
	{
		MainAnim->OnMontageEnded.AddDynamic(this, &AMainCharacter::OnAttackMontageEnded);
	}

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMainCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
}

void AMainCharacter::UpDown(float InputValue)
{
	if (bIsAttacking)
		return;

	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, InputValue);
}

void AMainCharacter::LeftRight(float InputValue)
{
	if (bIsAttacking)
		return;

	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, InputValue);
}

void AMainCharacter::Attack()
{
	if (bIsAttacking)
		return;

	if (MainAnim)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), AttackSound);

		MainAnim->PlayAttackMontage();
		MainAnim->JumpToSection(AttackSectionIndex);
		AttackSectionIndex = (AttackSectionIndex + 1) % 4;
	}

	bIsAttacking = true;

}

void AMainCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}

