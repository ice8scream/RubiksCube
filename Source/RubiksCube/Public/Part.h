// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Part.generated.h"

UENUM()
enum class CubeSideColor : int8 {
	Red, Orange, Yellow, Green, Blue, Purple
};



UCLASS()
class RUBIKSCUBE_API APart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APart();
	//APart(int8 plateCount/* = 3*/, TArray<CubeSideColor> plateColors/* = { CubeSideColor::Red, CubeSideColor::Orange, CubeSideColor::Yellow }*/);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SuperMesh;
	TArray<UStaticMeshComponent*> Plates;
	TArray<FString> CubeSideColorName = { "Red", "Orange", "Yellow", "Green", "Blue", "Purple" };

	void CreatePartCore();
	void AddPlates(
		int8 plateCount = 3,
		TArray<CubeSideColor> plateColors =
		{
			CubeSideColor::Red,
			CubeSideColor::Orange,
			CubeSideColor::Yellow
		});
};
