// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Side.h"
#include "Cube.generated.h"

UCLASS()
class RUBIKSCUBE_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMesh* PartObject;
	UStaticMesh* PlateObject;

private:

	TArray<ASide> sides;

	float GetAngleToRotate(FVector2D flatPos, float Z);

	class Part {
	public:
		Part();
		Part(UStaticMeshComponent& part);
		void RotatePart(FRotator addRotation);
		void PlacePart(FVector moveOnVector);

		UStaticMeshComponent* PartRoot;

		static int8 NumOfParts;
		static int8 NumOfPlates;

	};

	Part CreatePart(FVector Position);
};
