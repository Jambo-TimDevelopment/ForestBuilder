

#pragma once

#include "CoreMinimal.h"
#include "Core/FCBiomesMaskBuilder.h"
#include "FCBiomesPerlinMaskBuilder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCBiomesPerlinMaskBuilder : public UFCBiomesMaskBuilder
{
	GENERATED_BODY()
public:
	virtual void CreateBiomesMask(FLandscapeParameters& Parameters, TArray<uint8>& BiomesMask) override;

	// Repeat every 1000 Unreal units.
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0.0f, ClampMax = 1.0f))
	float PerlinNoiseScale = 0.1f;
};
