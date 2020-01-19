// Fill out your copyright notice in the Description page of Project Settings.


#include "Part.h"

#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

UPart::UPart() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
//}
//
//// Sets default values
//APart::APart(int8 plateCount, TArray<CubeSideColor> plateColors) : APart()
//{

	CreatePartCore();
	AddPlates();

}

void UPart::CreatePartCore() {
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Part"));


	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Game/parts/SidePart.SidePart"));

	if (CubeAsset.Succeeded())
	{
		SuperMesh->SetStaticMesh(CubeAsset.Object);

		static ConstructorHelpers::FObjectFinder<UMaterialInterface> PartColor(TEXT("/Game/PlateColors/CoreColor.CoreColor"));
		if (PartColor.Succeeded()) {
			SuperMesh->SetMaterial(0, PartColor.Object);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SidePart not found!"))
	}
}

void UPart::AddPlates(int8 plateCount, TArray<CubeSideColor> plateColors) {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlateAsset(TEXT("/Game/parts/PartPlate.PartPlate"));

	if (PlateAsset.Succeeded()) {

		FRotator rotation;

		for (int8 i = 0; i < plateCount; i++) {
			UE_LOG(LogTemp, Warning, TEXT("-----%d"), i);
			FName PlateName(*(TEXT("Plate" + FString::FromInt(i + 1))));

			auto topPlate = CreateDefaultSubobject<UStaticMeshComponent>(PlateName);
			Plates.Push(topPlate);
			auto pc = plateColors[i];

			UE_LOG(LogTemp, Warning, TEXT("%s"), *PlateName.ToString());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *(CubeSideColorName[(uint8)pc]));

			topPlate->SetStaticMesh(PlateAsset.Object);
			topPlate->AttachToComponent(SuperMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, PlateName);
			topPlate->SetRelativeScale3D({ 0.9,0.9,0.1 });
			FString PlateColorName = CubeSideColorName[(uint8)pc];
			ConstructorHelpers::FObjectFinder<UMaterialInterface> PlateColor(*(TEXT("/Game/PlateColors/" + PlateColorName + "." + PlateColorName)));
			if (PlateColor.Succeeded()) {
				topPlate->SetMaterial(0, PlateColor.Object);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Color %s not found!"), *PlateColorName);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NotFound"));
	}
}

// Called when the game starts or when spawned
void UPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UPart::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
