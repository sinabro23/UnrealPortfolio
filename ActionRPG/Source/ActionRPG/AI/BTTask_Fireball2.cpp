// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fireball2.h"
#include "Gideon.h"
#include "GideonAIController.h"

UBTTask_Fireball2::UBTTask_Fireball2()
{
	bNotifyTick = true;
	NodeName = TEXT("FireballAttack2");
}

EBTNodeResult::Type UBTTask_Fireball2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	AGideon* Gideon = Cast<AGideon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Gideon)
		return EBTNodeResult::Failed;

	Gideon->FireFireBall2();
	IsAttacking = true;
	Gideon->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Fireball2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

