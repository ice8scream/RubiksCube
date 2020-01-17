// Fill out your copyright notice in the Description page of Project Settings.


#include "Part.h"

#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
APart::APart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part"));
	SuperMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Game/parts/SidePart.SidePart"));

	if (CubeAsset.Succeeded()) 
	{
		SuperMesh->SetStaticMesh(CubeAsset.Object);
	} 
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("SidePart not found!"))
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> PlateAsset(TEXT("/Game/parts/PartPlate.PartPlate"));

	Plates.Push(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate")));


	
	if (PlateAsset.Succeeded()) {

		auto topPlate = Plates.Top();
		topPlate->SetStaticMesh(PlateAsset.Object);
		topPlate->AttachToComponent(SuperMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Plate1"));
		topPlate->SetRelativeScale3D({ 0.9,0.9,0.1 });
	}

	
	
}

// Called when the game starts or when spawned
void APart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

