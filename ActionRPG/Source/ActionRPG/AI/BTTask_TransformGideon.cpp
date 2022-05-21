// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TransformGideon.h"
#include "Gideon.h"	
#include "GideonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_TransformGideon::UBTTask_TransformGideon()
{
	bNotifyTick = true;
	NodeName = TEXT("GideonTransform");
}

EBTNodeResult::Type UBTTask_TransformGideon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AGideon* Gideon = Cast<AGideon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Gideon)
		return EBTNodeResult::Failed;

	if (true == OwnerComp.GetBlackboardComponent()->GetValueAsBool(AGideonAIController::IsSecondPageIn))
		return EBTNodeResult::Succeeded;

	

	Gideon->TransformPage2();
	Gideon->SetCanBeAttacked(false);
	IsAttacking = true;
	Gideon->OnTransformEndGideon.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_TransformGideon::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
