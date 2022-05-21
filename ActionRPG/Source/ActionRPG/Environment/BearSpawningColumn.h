// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawningColumn.h"
#include "BearSpawningColumn.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API ABearSpawningColumn : public ASpawningColumn
{
	GENERATED_BODY()
	
public:
	ABearSpawningColumn();
	virtual void SpawnMonster() override;
};
