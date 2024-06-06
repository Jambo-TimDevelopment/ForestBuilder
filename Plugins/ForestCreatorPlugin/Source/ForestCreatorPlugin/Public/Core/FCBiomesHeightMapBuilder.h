

#pragma once

#include "CoreMinimal.h"
#include "ForestCreator.h"
#include "FCBiomesHeightMapBuilder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCBiomesHeightMapBuilder : public UObject
{
	GENERATED_BODY()

public:
	virtual void CreateBiomesHeightMap(const FLandscapeParameters& Parameters, const TArray<uint8>& BiomesMask, OUT TArray<uint16>& BiomesHeightMap, uint8 CurrentBiomesIndex);
};
