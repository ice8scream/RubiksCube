// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CubeSideColor.h"

#include "Part.generated.h"


UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class RUBIKSCUBE_API UPart : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPart();
	//APart(int8 plateCount/* = 3*/, TArray<CubeSideColor> plateColors/* = { CubeSideColor::Red, CubeSideColor::Orange, CubeSideColor::Yellow }*/);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
