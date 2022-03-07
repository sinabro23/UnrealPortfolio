// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_TurnToCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTaskNode_TurnToCharacter : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_TurnToCharacter();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
