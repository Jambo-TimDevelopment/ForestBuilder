

#pragma once

#include "CoreMinimal.h"
#include "ForestCreator.h"
#include "UObject/NoExportTypes.h"
#include "FCRiverBuilder.generated.h"

class ALandscape;
struct FSplinePoint;
class ALandscapeBlueprintBrush;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCRiverBuilder : public UObject
{
	GENERATED_BODY()

public:
	void CreateRiver(const FLandscapeParameters& LandscapeParameters, ALandscape* Landscape);

private:
	void CreateRiverPoint(const FLandscapeParameters& LandscapeParameters, OUT TArray<FSplinePoint>& SplinePoints);

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RiverBrushClass;

	UPROPERTY(EditAnywhere)
	uint16 CountSplinePointsInRiver = 5;
};
