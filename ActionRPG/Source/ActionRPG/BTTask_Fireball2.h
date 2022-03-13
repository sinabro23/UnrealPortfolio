// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Fireball2.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTask_Fireball2 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Fireball2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;
};
