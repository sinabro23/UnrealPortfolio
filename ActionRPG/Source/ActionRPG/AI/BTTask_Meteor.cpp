// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Meteor.h"
#include "Gideon.h"
#include "GideonAIController.h"

UBTTask_Meteor::UBTTask_Meteor()
{
	bNotifyTick = true;
	NodeName = TEXT("Meteor");
}

EBTNodeResult::Type UBTTask_Meteor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	AGideon* Gideon = Cast<AGideon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Gideon)
		return EBTNodeResult::Failed;

	Gideon->FireMeteor();
	IsAttacking = true;
	Gideon->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Meteor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
