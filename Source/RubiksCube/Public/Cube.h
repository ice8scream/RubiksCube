// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY(EditAnywhere)
		UStaticMesh* PartObject;

	UPROPERTY(EditAnywhere)
		UStaticMesh* PlateObject;

	UPROPERTY(EditAnywhere)
		UMaterial* PartMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* RotationCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* InputHandleBox;

	TArray<FString> CubeSideColorName = { "Red", "Orange", "Yellow", "Green", "Blue", "Purple" };
	TArray<UMaterial*> sideColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UStaticMeshComponent*> Parts;

	UFUNCTION(BlueprintCallable)
		void AttachToRotateCenter(int32 PartIndex);

	UFUNCTION(BlueprintCallable)
		void AttachSideToRotatationCenter(int32 RotationAxisIndex, int32 RotationRowIndex);

private:

	float GetAngleToRotate(FVector2D flatPos, float Z);


	UStaticMeshComponent* CreatePart(FVector Position);
};
