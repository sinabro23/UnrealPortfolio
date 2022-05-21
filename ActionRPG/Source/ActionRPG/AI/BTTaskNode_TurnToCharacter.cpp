// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_TurnToCharacter.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_TurnToCharacter::UBTTaskNode_TurnToCharacter()
{
	NodeName = TEXT("TurnToCharacter");
}

EBTNodeResult::Type UBTTaskNode_TurnToCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
		return EBTNodeResult::Failed;

	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Monster->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Monster->SetActorRotation(FMath::RInterpTo(Monster->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.f));

	return EBTNodeResult::Succeeded;
}
