// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTASK_SingleProjectileAttack.h"
#include "Gideon.h"
#include "GideonAIController.h"
UBTTASK_SingleProjectileAttack::UBTTASK_SingleProjectileAttack()
{
	bNotifyTick = true;
	NodeName = TEXT("FireballAttack");
}

EBTNodeResult::Type UBTTASK_SingleProjectileAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	AGideon* Gideon = Cast<AGideon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Gideon)
		return EBTNodeResult::Failed;

	Gideon->FireBall();
	IsAttacking = true;
	Gideon->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTASK_SingleProjectileAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
