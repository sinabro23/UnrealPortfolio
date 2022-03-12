// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CharacterInBossRoom.h"
#include "Kismet/GameplayStatics.h"
#include "GideonAIController.h"
#include "MainCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gideon.h"

UBTService_CharacterInBossRoom::UBTService_CharacterInBossRoom()
{
	NodeName = TEXT("IsCharacterIn");
	Interval = 5.f;
}

void UBTService_CharacterInBossRoom::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;
	
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(ControllingPawn->GetWorld(), 0));
	if (Character->GetController()->IsPlayerController())
	{
		if (Character->IsCharacterInBossRoom())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGideonAIController::TargetKey, Character);
		}

		if (Character->IsCharacterInBossRoom())
		{
			auto Gideon = Cast<AGideon>(ControllingPawn);
			if (Gideon)
			{
				Gideon->SetHPBarVisiblity(true);
			}
		}
	
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AGideonAIController::HomePosKey);
	FNavLocation NextPatrolLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1000.f, NextPatrolLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AGideonAIController::RandomPosKey, NextPatrolLocation.Location);
	}

}
