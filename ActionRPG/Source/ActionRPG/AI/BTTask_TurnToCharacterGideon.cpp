// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToCharacterGideon.h"
#include "Gideon.h"
#include "MainCharacter.h"
#include "GideonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToCharacterGideon::UBTTask_TurnToCharacterGideon()
{
	NodeName = TEXT("TurnToCharacterGideon");
}

EBTNodeResult::Type UBTTask_TurnToCharacterGideon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Gideon = Cast<AGideon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Gideon)
		return EBTNodeResult::Failed;

	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGideonAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Gideon->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Gideon->SetActorRotation(TargetRot);

	return EBTNodeResult::Succeeded;
}
