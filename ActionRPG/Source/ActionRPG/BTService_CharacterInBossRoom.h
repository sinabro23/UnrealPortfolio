// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CharacterInBossRoom.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTService_CharacterInBossRoom : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CharacterInBossRoom();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
