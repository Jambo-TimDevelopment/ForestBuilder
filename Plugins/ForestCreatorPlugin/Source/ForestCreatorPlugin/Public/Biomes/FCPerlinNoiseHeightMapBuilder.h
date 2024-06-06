

#pragma once

#include "CoreMinimal.h"
#include "Core/FCBiomesHeightMapBuilder.h"
#include "FCPerlinNoiseHeightMapBuilder.generated.h"

USTRUCT(BlueprintType)
struct FOctave
{
	GENERATED_BODY()

	FOctave()
	{
		PerlinNoiseScale = 0.1f;
		HeightScale = 1;
	}

	// Repeat every 1000 Unreal units.
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0.0f, ClampMax = 1.0f))
	float PerlinNoiseScale = 0.1f;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0.0f))
	float HeightScale = 1;
};

UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCPerlinNoiseHeightMapBuilder : public UFCBiomesHeightMapBuilder
{
	GENERATED_BODY()

public:
	virtual void CreateBiomesHeightMap(const FLandscapeParameters& Parameters, const TArray<uint8>& BiomesMask,OUT TArray<uint16>& BiomesHeightMap, uint8 CurrentBiomesIndex) override;

private:
	float GetHeightByOctaves(int X, int Y);

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FOctave> Octaves;
};
