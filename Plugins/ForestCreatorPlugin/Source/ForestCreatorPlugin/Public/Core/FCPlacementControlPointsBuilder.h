

#pragma once

#include "CoreMinimal.h"
#include "ForestCreator.h"
#include "UObject/NoExportTypes.h"
#include "FCPlacementControlPointsBuilder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCPlacementControlPointsBuilder : public UObject
{
	GENERATED_BODY()

public:
	void CreateControlPoint(UFCNavigationGridBuilder* NavigationGridBuilder, TMap<int32, AFCControlPoint*> NumberControlPoints);

	UPROPERTY(EditDefaultsOnly, Category="ControlPoints")
	TArray<int32> ControlPointIndexes;

	UPROPERTY(EditDefaultsOnly, Category="ControlPoints")
	int32 StartControlPointIndex;

	UPROPERTY(EditDefaultsOnly, Category="ControlPoints")
	TSubclassOf<AFCControlPoint> ControlPointClass;
};
