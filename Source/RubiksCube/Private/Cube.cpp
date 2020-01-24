// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
int8 ACube::Part::NumOfParts = 0;
int8 ACube::Part::NumOfPlates = 0;

ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Создаем дефолтный компонент и присваеваем его в рут куба
	auto topPart = CreateDefaultSubobject<UStaticMeshComponent>("CubeCenter");
	SetRootComponent(topPart);

	// Получаем ассеты форм и базового цвета из файлов
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		PartAsset(TEXT("/Game/parts/SidePart.SidePart"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		PlateAsset(TEXT("/Game/parts/PartPlate.PartPlate"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		PartColor(TEXT("Material'/Game/PlateColors/CoreColor.CoreColor'"));

	// Сохраняем ассеты в члены класса
	PartObject = PartAsset.Object;
	PlateObject = PlateAsset.Object;
	PartMaterial = PartColor.Object->GetMaterial();

	// Получаем ассеты цветов для плиток и добавляем их в масив для цветов
	for (uint8 i = 0; i < CubeSideColorName.Num(); i++) {
		ConstructorHelpers::FObjectFinder<UMaterialInterface> 
			PlateColor(*(TEXT("/Game/PlateColors/" + CubeSideColorName[i] + "." + CubeSideColorName[i])));
		
		sideColors.Push(PlateColor.Object->GetMaterial());
	}
	
	// Генерируем части куба
	for (uint8 z = 0; z < 3; z++) {
		for (uint8 x = 0; x < 3; x++) {
			for (uint8 y = 0; y < 3; y++) {
				CreatePart({ (float)x, (float)y, (float)z });
			}
		}
	}

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
	FVector2D startPos(-1, startY);
	float AngleToRotateCos = FVector2D::DotProduct(startPos, flatPos) / 
		(startPos.Size() * flatPos.Size());
	float AngleToRotate = FMath::RadiansToDegrees(FMath::Acos(AngleToRotateCos));

	if (flatPos.Y > 0 ||
		(startY == 1 && flatPos.X == 1) ||
		(startY == -1 && flatPos == FVector2D(-1,0))
		) { AngleToRotate = 360 - AngleToRotate; }

	AngleToRotate = floor(AngleToRotate / 90) * 90;

	return AngleToRotate;
}

ACube::Part ACube::CreatePart(FVector Position) {

	uint8 plateQuantity = 
		((uint8)Position.X + 1) % 2 + 
		((uint8)Position.Y + 1) % 2 + 
		((uint8)Position.Z + 1) % 2;

	Position += FVector(-1);
	Position.Z *= -1;

	FString newPartName("Part" + FString::FromInt(Part::NumOfParts++));
	Part newPart(*CreateDefaultSubobject<UStaticMeshComponent>(*newPartName)); 

	newPart.PartRoot->SetStaticMesh(PartObject);
	newPart.PartRoot->SetMaterial(0,PartMaterial);


	
	newPart.PartRoot->AttachToComponent(RootComponent,
		FAttachmentTransformRules::SnapToTargetIncludingScale);


	float YawAngle = GetAngleToRotate({ Position.X, Position.Y }, Position.Z);
	newPart.RotatePart(FRotator(0, YawAngle, (-Position.Z + 1) * -90));
	newPart.PlacePart(GetActorLocation() + Position);

	for (int8 i = 0; i < plateQuantity; i++) {
		FString newPlateName("Plate" + FString::FromInt(Part::NumOfPlates++));

		auto newPlate = CreateDefaultSubobject<UStaticMeshComponent>(*newPlateName);

		newPlate->SetStaticMesh(PlateObject);
		newPlate->AttachToComponent(newPart.PartRoot, FAttachmentTransformRules::SnapToTargetIncludingScale,
			*("Plate" + FString::FromInt(i + 1)));
		newPlate->SetRelativeScale3D({ 0.9,0.9,0.1 });

		auto plateUpVector = newPlate->GetUpVector();

		uint8 materialIndex = 0;


		auto v = newPlate->GetUpVector();
		
		for (uint8 j = 0; j < 3; j++) {
			auto roundPoint = FMath::RoundHalfFromZero(plateUpVector[j]);

			if (roundPoint == 1) { materialIndex++; }
			if (roundPoint != 0) { break; }
			materialIndex += 2;
		}
		newPlate->SetMaterial(0, sideColors[materialIndex]);
	}
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
