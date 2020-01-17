// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Part.h"
#include "Side.generated.h"


UCLASS()
class RUBIKSCUBE_API ASide : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASide();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateRight();
	void RotateLeft();

	void ChangeNaighborEdge(CubeSideColor naighbor, TArray<APart*>* edge);// TODO ice8scream :: make some logic

private:
	TMap<CubeSideColor,ASide*> naighborSides;
	TArray<TPair<CubeSideColor,TArray<APart*>*>> edges;
	APart* centerPart;

};
