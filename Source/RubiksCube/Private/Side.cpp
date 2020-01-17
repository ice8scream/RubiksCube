// Fill out your copyright notice in the Description page of Project Settings.


#include "Side.h"
#include "..\Public\Side.h"

// Sets default values
ASide::ASide()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASide::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASide::RotateRight()
{
	uint8 EdgeQuantity = edges.Num();
	auto BufferedSide = edges[EdgeQuantity-1].Key;
	for (uint8 i = 0; i < EdgeQuantity; i++)
	{
		Swap(BufferedSide, edges[i].Key);
		// TODO ice8scream :: change naighbor parts to
	}
}

void ASide::RotateLeft()
{
	auto BufferedSide = edges[0].Key;
	for (uint8 i = edges.Num() - 1; i >= 0; i--)
	{
		Swap(BufferedSide, edges[i].Key);
		// TODO ice8scream :: change naighbor parts to
	}
}


