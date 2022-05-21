// Fill out your copyright notice in the Description page of Project Settings.


#include "BearSpawningColumn.h"
#include "Bear.h"

ABearSpawningColumn::ABearSpawningColumn()
{
	DetectRange = 2000.f;
}

void ABearSpawningColumn::SpawnMonster()
{
	ABear* Bear = GetWorld()->SpawnActor<ABear>(GetRandomPoint() + FVector(0.0f, 0.0f, 100.f), FRotator::ZeroRotator);
}