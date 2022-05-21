// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToCharacterGideon.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTask_TurnToCharacterGideon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToCharacterGideon();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
