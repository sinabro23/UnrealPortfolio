// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectMainCharacter.h"
#include "MainCharacter.h"
#include "Monster.h"	
#include "MonsterAIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DetectMainCharacter::UBTService_DetectMainCharacter()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_DetectMainCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;
	auto Monster = Cast<AMonster>(ControllingPawn);

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.f;

	if (nullptr == World)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollsionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollsionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AMainCharacter* Character = Cast<AMainCharacter>(OverlapResult.GetActor());
			if (Character)
			{
				if (Character->GetController()->IsPlayerController())
				{
					
					if (Monster)
					{
						Monster->FindEnemy();
					}

					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, Character);
					//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.5f);
					//DrawDebugPoint(World, Character->GetActorLocation(), 10.f, FColor::Blue, false, 0.5f);
					//DrawDebugLine(World, Center, Character->GetActorLocation(), FColor::Blue, false, 0.5f);
					return;
				}
			}
		}
	}


	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, nullptr);
	Monster->MissEnemy();
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.5f);
}
