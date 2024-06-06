#pragma once

#include "CoreMinimal.h"
#include "AWaterGenerator.h"
#include "FBiome.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PerlinNoiseHeightMap.h"
#include "HeightMap.generated.h"

class  UProceduralMeshComponent;
class  UMaterialInterface	;

UCLASS()
class RUNFORESTVR_API AHeightMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeightMap();
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	int XSize = 0;
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	int YSize = 0;
	// масштаб сетки
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.000001))
	int Scale = 0;
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.000001))
	int UVScale = 1;
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.0000001))
	float ZMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, meta=(ClampMin = -1))
	float NoiseScale = 1.0f;
	virtual void BeginPlay() override;
protected:
	// Called when the game starts or when spawned
	
	virtual void OnConstruction(const FTransform& Transform) override;
	// материал
	UPROPERTY(EditAnywhere);
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere);
	UMaterialInterface* MaterialWater;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY()
	UProceduralMeshComponent* ProceduralMesh;
	UProceduralMeshComponent* ProceduralMeshWater;
	// переменные для нашей сетки
	//вершины
	TArray<FVector> Vertices;
	// треугольники формирующие плоскость
	TArray<int> Triangles;
	// текстура
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangens;
	PerlinNoiseHeightMap PerlinNoise;
	AWaterGenerator WaterGenerator;
	TArray<FVector> WaterForLand;
	TArray<FBiome> BiomeMapGaneration;
	void CreateVertices();
	void CreateTriangles();
	void CreateBiomesMap();
	// тип шума исходя из высоты
	float GetNoiseValue(float Heigth);
	// позволяет определить тип природной зоны исходя из высоты
	int GetTypeNatureZone(float Heigth);
	TArray<FVector> ParseRiver();
};