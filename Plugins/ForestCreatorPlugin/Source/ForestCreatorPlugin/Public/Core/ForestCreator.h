

#pragma once

#include "CoreMinimal.h"
#include "FCNavigationGridBuilder.h"
#include "GameFramework/Actor.h"
#include "ForestCreator.generated.h"

class UFCNavigationGridBuilder;
class UFCPlacementControlPointsBuilder;
class UFCRiverBuilder;
class AFCControlPoint;
class UFCHeightMapSmoothingFilter;
class UFCBiomesDataAsset;
class UFCBiomesMaskBuilder;
struct FLandscapeImportLayerInfo;

USTRUCT()
struct FLandscapeParameters
{
	GENERATED_BODY()

	int32 SectionSize = 63;

	int32 SectionsPerComponent = 1;

	int32 ComponentCountX = 8;

	int32 ComponentCountY = 8;

	FVector SectionScale = FVector(100.0f);

	FTransform LandscapeTransform;

	float GlobalHeightScale;

	UMaterialInterface* LandscapeMaterial;

	TSet<UFCBiomesDataAsset*> BiomesSet;

	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;

	TMap<FGuid, TArray<FLandscapeImportLayerInfo>*> MaterialLayerDataPerLayers;

	TArray<uint16> HeightData;

	TArray<float> GlobalHeightData;

	TArray<uint8> BiomesMask;
};

/**
* The main actor responsible for creating a forest.  
* Drop this actor into your scene, assign a theme and click "Build forest" button
* to create your forest.  
* From code, call AForestCreator::BuildForest after adding an entry into the AForestCreator::Themes array
*/
UCLASS(NotBlueprintable, hidecategories = (Rendering,Input,Actor,Misc,Replication,Collision,LOD,Cooking))
class FORESTCREATORPLUGIN_API AForestCreator : public AActor
{
	GENERATED_BODY()

public:
	AForestCreator(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = ForestCreator)
	void BuildForest();

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	TSet<UFCBiomesDataAsset*> GetBiomesSet();

protected:
	UPROPERTY(EditAnywhere, Category=ForestCreator)
	TSubclassOf<UFCBiomesMaskBuilder> BiomesMaskBuilderClass;

	UPROPERTY(EditAnywhere, Category=ForestCreator)
	TSubclassOf<UFCPlacementControlPointsBuilder> ControlPointBuilderClass;

	UPROPERTY(EditAnywhere, Category=ForestCreator)
	TSubclassOf<UFCHeightMapSmoothingFilter> HeightMapSmoothingFilterClass;

	UPROPERTY(EditAnywhere, Category=ForestCreator)
	TSet<UFCBiomesDataAsset*> BiomesSet;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	int32 SectionSize = 63;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Seed")
	int32 Seed = 0;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	int32 SectionsPerComponent = 1;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	int32 ComponentCountX = 8;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	int32 ComponentCountY = 8;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	FVector SectionScale = FVector(100.0f);

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	int32 MaxStepHeightToLerp = 100;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	float GlobalHeightScale = 5.5f;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Landscape parameter")
	UMaterialInterface* LandscapeMaterial;

public:
	UPROPERTY(EditAnywhere, Category="ForestCreator|Orienteering parameter")
	TMap<int32, AFCControlPoint*> ControlPoints;

public:
	TArray<FPointNavigationGrid> GlobalWay;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Orienteering parameter")
	TSubclassOf<UFCNavigationGridBuilder> NavigationGridBuilderClass;

	UPROPERTY(EditAnywhere, Category="ForestCreator|Rivers parameter")
	TSubclassOf<UFCRiverBuilder> RiverClass;

	FLandscapeParameters LandscapeParameters;

	bool bLandscapeWacCreated = false;
};
