

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FCBiomesMaskBuilder.generated.h"

struct FLandscapeParameters;

/**
 * The base class for generate biomes map
 */
UCLASS()
class FORESTCREATORPLUGIN_API UFCBiomesMaskBuilder : public UObject
{
	GENERATED_BODY()

public:
	virtual void CreateBiomesMask(FLandscapeParameters& Parameters,
	                              OUT TArray<uint8>& BiomesMask /* TODO TSet<UFCBiomesDataAsset> BiomesDataAssets*/);

	// TODO create method CreateHeightMapDataByBiomes(const FLandscapeParameters& Parameters, const FBiomesData& Biomes)
};
