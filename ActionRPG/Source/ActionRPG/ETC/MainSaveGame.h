// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainSaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentHP;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentMP;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxMP;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float CurrentStamina;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;
};

UCLASS()
class ACTIONRPG_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMainSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats;

};
