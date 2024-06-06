

#pragma once

#include "CoreMinimal.h"
#include "ForestCreator.h"
#include "UObject/NoExportTypes.h"
#include "FCHeightMapSmoothingFilter.generated.h"

/** Base class for realise smooth algorithm for height map */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCHeightMapSmoothingFilter : public UObject
{
	GENERATED_BODY()

public:
	void ApplyHeightMapSmoothingFilter(const FLandscapeParameters& Parameters, OUT TArray<uint16>& BiomesHeightMap);

private:
	float GetSmoothingHeight(const TArray<uint16>& BiomesHeightMap, int32 X, int32 Y, int32 i);

protected:
	UPROPERTY(EditDefaultsOnly)
	float MinStepForApplyFilter = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float CubicInterpolationAlpha = 0.1f;
};
