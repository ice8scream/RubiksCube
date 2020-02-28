// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Создаем дефолтный компонент и присваеваем его в рут куба
	auto topPart = CreateDefaultSubobject<USceneComponent>("CubeCenter");
	SetRootComponent(topPart);
	RotationCenter = CreateDefaultSubobject<USceneComponent>("RotationCenter");

	RotationCenter->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	if (IsValid(RotationCenter)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("InValid"));
	}

	InputHandleBox = CreateDefaultSubobject<UBoxComponent>("Cube Handle Box");
	InputHandleBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InputHandleBox->InitBoxExtent(FVector(150)); // TODO ice8scream :: maybe mace const
	InputHandleBox->ComponentTags.Push("Cube Handle Box");

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
	for (uint8 z = 0; z < 3; z++)
	{
		for (uint8 y = 0; y < 3; y++)
		{
			for (uint8 x = 0; x < 3; x++)
			{
				Parts.Push(CreatePart({ (float)x, (float)y, (float)z }));
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

void ACube::AttachToRotateCenter(int32 PartIndex)
{
	if (IsValid(Parts[PartIndex]))
	{
		bool what = Parts[PartIndex]->AttachToComponent(RotationCenter, FAttachmentTransformRules::KeepRelativeTransform);
		if (!what)
		{
			UE_LOG(LogTemp, Error, TEXT("Part is not attached"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Part is invalid"));
	}
}

void ACube::RotateSide(TArray<int32> Side)
{

	UE_LOG(LogTemp, Error, TEXT("%d, %d, %d"), Side[0], Side[1], Side[2]);

	TArray<int32> MinIndex, MaxIndex;
	MinIndex.Init(0, Side.Num());
	MaxIndex.Init(2, Side.Num());

	for (int32 i = 0; i < Side.Num(); i++)
	{
		if (Side[i] != -1)
		{
			MinIndex[i] = Side[i];
			MaxIndex[i] = Side[i];
		}
	}

	for (int32 x = MinIndex[0]; x <= MaxIndex[0]; x++)
	{
		for (int32 y = MinIndex[1]; y <= MaxIndex[1]; y++)
		{
			for (int32 z = MinIndex[2]; z <= MaxIndex[2]; z++)
			{
				int32 PartIndex = x + 3 * y + 9 * z;
				AttachToRotateCenter(PartIndex);
			}
		}
	}

	for (auto child : Parts)
	{
		UE_LOG(LogTemp, Error, TEXT("Parent component is %s"), *(child->GetAttachParent()->GetName())); // TODO ice8scream :: make var
	}

}

float ACube::GetAngleToRotate(FVector2D flatPos, float Z)
{
	if (flatPos.IsZero())
	{
		return 0;
	}

	float startY = (uint8)(Z + 3) % 3 - 1;
	FVector2D startPos(-1, startY);
	float AngleToRotateCos = FVector2D::DotProduct(startPos, flatPos) /	(startPos.Size() * flatPos.Size()); // TODO ice8scream :: make var
	float AngleToRotate = FMath::RadiansToDegrees(FMath::Acos(AngleToRotateCos));

	if (flatPos.Y > 0 ||
		(startY == 1 && flatPos.X == 1) ||
		(startY == -1 && flatPos == FVector2D(-1,0))
		) { AngleToRotate = 360 - AngleToRotate; }  // TODO ice8scream :: make var

	AngleToRotate = floor(AngleToRotate / 90) * 90;
	return AngleToRotate;
}

UStaticMeshComponent* ACube::CreatePart(FVector Position)
{
	uint8 plateQuantity = 
		((uint8)Position.X + 1) % 2 + 
		((uint8)Position.Y + 1) % 2 + 
		((uint8)Position.Z + 1) % 2;

	FString newPartName("Part" + FString::FromInt( Position.Z * 9 + Position.Y * 3 + Position.X	)); // TODO ice8scream :: make var

	Position += FVector(-1);
	Position.Z *= -1;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *newPartName); // TODO ice8scream :: delete

	// TODO ice8scream :: maybe make function----
	UStaticMeshComponent* newPart = CreateDefaultSubobject<UStaticMeshComponent>(*newPartName); 

	newPart->SetStaticMesh(PartObject);
	newPart->SetMaterial(0, PartMaterial);

	newPart->AttachToComponent(RootComponent,
		FAttachmentTransformRules::SnapToTargetIncludingScale);

	float YawAngle = GetAngleToRotate({ Position.X, Position.Y }, Position.Z);
	newPart->SetRelativeRotation({0, YawAngle, (-Position.Z + 1) * -90});
	newPart->SetRelativeLocation((GetActorLocation() + Position)*100);
	//------------------------

	for (int8 i = 0; i < plateQuantity; i++) 
	{
		FString newPlateName("Plate" + newPartName + "-"+ FString::FromInt(i));
		auto newPlate = CreateDefaultSubobject<UStaticMeshComponent>(*newPlateName);
		newPlate->SetStaticMesh(PlateObject);
		newPlate->AttachToComponent(newPart, FAttachmentTransformRules::SnapToTargetIncludingScale,
			*("Plate" + FString::FromInt(i + 1)));
		newPlate->SetRelativeScale3D({ 0.9,0.9,0.1 });
		auto plateUpVector = newPlate->GetUpVector();
		uint8 materialIndex = 0;
		auto v = newPlate->GetUpVector();
		for (uint8 j = 0; j < 3; j++) 
		{
			auto roundPoint = FMath::RoundHalfFromZero(plateUpVector[j]);
			if (roundPoint == 1)
			{ 
				materialIndex++;
			}
			if (roundPoint != 0)
			{
				break;
			}
			materialIndex += 2;
		}
		newPlate->SetMaterial(0, sideColors[materialIndex]);
	}
	return newPart;
}
