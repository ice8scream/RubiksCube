// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
//#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values

int8 ACube::Part::NumOfParts = 0;
int8 ACube::Part::NumOfPlates = 0;

ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto topPart = CreateDefaultSubobject<UStaticMeshComponent>("CubeCenter");
	SetRootComponent(topPart);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> PartAsset(TEXT("/Game/parts/SidePart.SidePart"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlateAsset(TEXT("/Game/parts/PartPlate.PartPlate"));

	PartObject = PartAsset.Object;
	PlateObject = PlateAsset.Object;

	for (uint8 z = 0; z < 3; z++) {
		for (uint8 x = 0; x < 3; x++) {
			for (uint8 y = 0; y < 3; y++) {
				CreatePart({ (float)x, (float)y, (float)z });
			}
		}
	}

	//CreatePart({0,0,0});
	//UE_LOG(LogTemp, Error, TEXT("%f"), GetAngleToRotate({-1,1}));
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACube::GetAngleToRotate(FVector2D flatPos, float Z)
{
	if (flatPos.IsZero()) {
		return 0;
	}
	float startY = (uint8)(Z + 3) % 3 - 1;

	UE_LOG(LogTemp, Error, TEXT("%f -> %f"), Z, startY);
	FVector2D startPos(-1, startY);
	float AngleToRotate = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(startPos, flatPos) / (startPos.Size() * flatPos.Size())));
	//UE_LOG(LogTemp, Error, TEXT("%f"), AngleToRotate);
	if (flatPos.Y > 0 ||
		(startY == 1 && flatPos.X == 1) ||
		(startY == -1 && flatPos == FVector2D(-1,0))
		) { AngleToRotate = 360 - AngleToRotate; }
	AngleToRotate = floor(AngleToRotate / 90) * 90;
	return AngleToRotate;
}

ACube::Part ACube::CreatePart(FVector Position) {

	uint8 plateQuantity = ((uint8)Position.X + 1) % 2 + ((uint8)Position.Y + 1) % 2 + ((uint8)Position.Z + 1) % 2;
	//UE_LOG(LogTemp, Error, TEXT("%d"), plateQuantity);

	bool visible = true;//Position.Z == 1;//true;// plateQuantity < 2;

	if (visible) {
		//UE_LOG(LogTemp, Error, TEXT("%f %f %f -> %d"), Position.X, Position.Y, Position.Z, plateQuantity);
	}

	Position += FVector(-1);
	Position.Z *= -1;


	auto topPart = CreateDefaultSubobject<UStaticMeshComponent>(*("Part" + FString::FromInt(Part::NumOfParts++)));

	topPart->SetStaticMesh(PartObject);

	for (int8 i = 0; i < plateQuantity; i++) {
		FName PlateName(*(TEXT("Plate" + FString::FromInt(Part::NumOfPlates++))));

		auto topPlate = CreateDefaultSubobject<UStaticMeshComponent>(PlateName);
		//Plates.Push(topPlate);
		//auto pc = plateColors[i];

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *PlateName.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *(CubeSideColorName[(uint8)pc]));

		topPlate->SetStaticMesh(PlateObject);
		topPlate->AttachToComponent(topPart, FAttachmentTransformRules::SnapToTargetIncludingScale, *("Plate" + FString::FromInt(i + 1)));
		topPlate->SetRelativeScale3D({ 0.9,0.9,0.1 });
		//FString PlateColorName = CubeSideColorName[(uint8)pc];
		//ConstructorHelpers::FObjectFinder<UMaterialInterface> PlateColor(*(TEXT("/Game/PlateColors/" + PlateColorName + "." + PlateColorName)));
		/*if (PlateColor.Succeeded()) {
			topPlate->SetMaterial(0, PlateColor.Object);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Color %s not found!"), *PlateColorName);
		}*/
		topPlate->SetVisibility(visible);
	}
	
	topPart->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	Part newPart(*topPart);

	float YawAngle = GetAngleToRotate({ Position.X, Position.Y }, Position.Z);
	//UE_LOG(LogTemp, Error, TEXT("%f, %f"), YawAngle, (-Position.Z + 1) * -90);
	newPart.RotatePart(FRotator(0, YawAngle, (-Position.Z + 1) * -90));
	newPart.PlacePart(GetActorLocation() + Position);

	newPart.PartRoot->SetVisibility(visible);

	return newPart;
}


ACube::Part::Part() {
	
}

ACube::Part::Part(UStaticMeshComponent& part)
{
	PartRoot = &part;
}

void ACube::Part::RotatePart(FRotator addRotation)
{
	PartRoot->AddRelativeRotation(addRotation);
}

void ACube::Part::PlacePart(FVector moveOnVector)
{
	PartRoot->AddRelativeLocation(moveOnVector * 100);
}
