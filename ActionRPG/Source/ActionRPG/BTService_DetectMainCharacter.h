// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectMainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTService_DetectMainCharacter : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DetectMainCharacter();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
