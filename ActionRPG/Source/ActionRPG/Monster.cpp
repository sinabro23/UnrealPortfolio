// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterAIController.h"
#include "MonsterHealthbarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

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
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 300.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW_HPBAR(TEXT("WidgetBlueprint'/Game/_Game/UI/Monster/MonsterHealthBar.MonsterHealthBar_C'"));
	if (UW_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UW_HPBAR.Class);
		HPBar->SetDrawSize(FVector2D(300.f, 50.f));
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));
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
}

void AMonster::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	if (CurrentHP < 0)
		CurrentHP = 0;

	OnHPChanged.Broadcast();
}

float AMonster::GetHPRatio()
{
	return CurrentHP / MaxHP;
}


