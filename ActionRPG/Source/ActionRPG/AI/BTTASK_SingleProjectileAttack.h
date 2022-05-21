// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTASK_SingleProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTASK_SingleProjectileAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTASK_SingleProjectileAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;
};
