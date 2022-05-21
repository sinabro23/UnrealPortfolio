// Fill out your copyright notice in the Description page of Project Settings.


#include "GideonAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MainCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AGideonAIController::HomePosKey(TEXT("HomePos"));
const FName AGideonAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AGideonAIController::TargetKey(TEXT("Target"));
const FName AGideonAIController::RandomPosKey(TEXT("RandomPos"));
const FName AGideonAIController::IsPageChangedKey(TEXT("IsPageChanged"));
const FName AGideonAIController::IsSecondPageIn(TEXT("IsSecondPageIn"));

AGideonAIController::AGideonAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_GIDEON(TEXT("BehaviorTree'/Game/_Game/Gideon/AI/BT_Gideon.BT_Gideon'"));
	if (BT_GIDEON.Succeeded())
	{
		BTAsset = BT_GIDEON.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_GIDEON(TEXT("BlackboardData'/Game/_Game/Gideon/AI/BB_Gideon.BB_Gideon'"));
	if (BB_GIDEON.Succeeded())
	{
		BBAsset = BB_GIDEON.Object;
	}
}

void AGideonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("POSSESS BOSS"));
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			//UE_LOG(LogTemp, Warning, TEXT("behaviortree 작동 실패"));
		}
	}
}

void AGideonAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AGideonAIController::RunAI()
{
}

void AGideonAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
