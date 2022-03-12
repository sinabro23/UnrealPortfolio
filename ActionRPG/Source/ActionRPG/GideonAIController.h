// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GideonAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AGideonAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGideonAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName RandomPosKey;

protected:

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
