#include "HeightMap.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

#include "AWaterGenerator.h"
#include "Landscape.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHeightMap::AHeightMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMeshWater = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshWater");
	ProceduralMeshWater->SetupAttachment(GetRootComponent());
}

void AHeightMap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();
	BiomeMapGaneration.Reset();
	WaterGenerator.Water.Reset();

	CreateBiomesMap();
	CreateVertices();
	CreateTriangles();

	WaterGenerator.CreateRiver(BiomeMapGaneration, XSize, YSize, 3);
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangens);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(),
	                                  Tangens, true);
	ProceduralMesh->SetMaterial(0, Material);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(ParseRiver(), Triangles, UV0, Normals, Tangens);
	ProceduralMeshWater->CreateMeshSection(0, ParseRiver(), Triangles, TArray<FVector>(), TArray<FVector2d>(), TArray<FColor>(),
	                                       Tangens, true);
	ProceduralMeshWater->SetMaterial(0, MaterialWater);


	// ALandscape* MyLandscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(this, GetDefault<ALandscape>()->GetClass()));
	// MyLandscape->GetComponents()
}


// Called when the game starts or when spawned
void AHeightMap::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHeightMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FVector> AHeightMap::ParseRiver()
{
	for (int i = 0; i < WaterGenerator.Water.Num() - 10; i++)
	{
		if (WaterGenerator.Water[i].IsWater == 1)
		{
			WaterForLand.Add(FVector(WaterGenerator.Water[i].Coordinate.X, WaterGenerator.Water[i].Coordinate.Y, 500));
		}
		else WaterForLand.Add(FVector(WaterGenerator.Water[i].Coordinate.X, WaterGenerator.Water[i].Coordinate.Y, 0));
	}
	return WaterForLand;
}


void AHeightMap::CreateVertices()
{
	long const LengthXValue = XSize * YSize;
	TArray<float> XValue;
	for (int i = 0; i < LengthXValue; i++)
	{
		XValue.Add(FMath::RandRange(1, 4));
	}

	for (int X = 0; X <= XSize; ++X)
	{
		for (int Y = 0; Y <= YSize; ++Y)
		{
			if (X < XSize && Y < YSize)
			{
				float NowValue = BiomeMapGaneration[X * YSize + Y].Coordinate.Z;
				NoiseScale = GetNoiseValue(NowValue);
			}
			else
			{
				NoiseScale = 0.1;
			}
			float Z = PerlinNoise.noise(X * NoiseScale + 0.1, Y * NoiseScale + 0.1, XValue, YSize) * ZMultiplier;
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}


void AHeightMap::CreateTriangles()
{
	int Vertex = 0;

	for (int X = 0; X < XSize; ++X)
	{
		for (int Y = 0; Y < YSize; ++Y)
		{
			Triangles.Add(Vertex); //Bottom left corner
			Triangles.Add(Vertex + 1); //Bottom right corner
			Triangles.Add(Vertex + YSize + 1); //Top left corner
			Triangles.Add(Vertex + 1); //Bottom right corner
			Triangles.Add(Vertex + YSize + 2); //Top right corner
			Triangles.Add(Vertex + YSize + 1); //Top left corner

			++Vertex;
		}
		++Vertex;
	}
}

void AHeightMap::CreateBiomesMap()
{
	NoiseScale = 0.1;
	long const LengthXBiomesMapValue = XSize * YSize;
	TArray<float> XBiomesValue;
	for (int i = 0; i < LengthXBiomesMapValue; i++)
	{
		XBiomesValue.Add(FMath::RandRange(1, 4));
	}
	for (int X = 0; X <= XSize; ++X)
	{
		for (int Y = 0; Y <= YSize; ++Y)
		{
			float Z = PerlinNoise.noise(X * NoiseScale + 0.1, Y * NoiseScale + 0.1, XBiomesValue, YSize) * ZMultiplier;
			// определение природной зоны
			int TypeNatureZone = GetTypeNatureZone(Z);
			BiomeMapGaneration.Add(FBiome(FVector(X * Scale, Y * Scale, Z), TypeNatureZone));
		}
	}
}

float AHeightMap::GetNoiseValue(float Heigth)
{
	if (Heigth <= -400)
	{
		return 0.5;
	}
	if (Heigth > -400 && Heigth < -100)
	{
		return 0.09;
	}
	if (Heigth >= -100 && Heigth < 250)
	{
		return 0.001;
	}
	if (Heigth >= 250 && Heigth < 350)
	{
		return 0.2;
	}
	return 0.9;
}

int AHeightMap::GetTypeNatureZone(float Heigth)
{
	if (Heigth <= -400)
	{
		// глубокий овраг
		return 0;
	}
	if (Heigth > -400 && Heigth < -100)
	{
		// впадины и озёра
		return 1;
	}
	if (Heigth >= -100 && Heigth < 250)
	{
		// равнина
		return 2;
	}
	if (Heigth >= 250 && Heigth < 350)
	{
		// лес
		return 3;
	}
	// холм
	return 4;
}