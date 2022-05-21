// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MainCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMonsterAIController::HomePosKey(TEXT("HomePos"));
const FName AMonsterAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMonsterAIController::TargetKey(TEXT("Target"));


AMonsterAIController::AMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTOBJECT(TEXT("BehaviorTree'/Game/_Game/Monster/Troll/AI/BT_Troll.BT_Troll'"));
	if (BTOBJECT.Succeeded())
	{
		BTAsset = BTOBJECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBOBJECT(TEXT("BlackboardData'/Game/_Game/Monster/Troll/AI/BB_Troll.BB_Troll'"));
	if (BBOBJECT.Succeeded())
	{
		BBAsset = BBOBJECT.Object;
	}
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonsterAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			//UE_LOG(LogTemp, Warning, TEXT("behaviortree 작동 실패"));
		}
	}
}

void AMonsterAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
