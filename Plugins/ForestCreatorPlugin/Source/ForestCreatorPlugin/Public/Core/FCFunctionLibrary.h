

#pragma once

#include "CoreMinimal.h"
#include "FCNavigationGridBuilder.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FCFunctionLibrary.generated.h"

class ALandscapeBlueprintBrushBase;
struct FLandscapeParameters;
class ALandscape;


UCLASS()
class FORESTCREATORPLUGIN_API UFCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AActor* FindObjectByClass(TSubclassOf<AActor> ActorClass);

	static ALandscape* CreateLandscape(const FLandscapeParameters& Parameters, ALandscape* LandscapeForUpdate);

	UFUNCTION(BlueprintCallable)
	static void UpdateOwningLandscape(ALandscapeBlueprintBrushBase* Brush, ALandscape* Landscape);

	static void FindingWayByDijkstra(UFCNavigationGridBuilder* UfcNavigationGridBuilder, FPointNavigationGrid Start, FPointNavigationGrid End);

	static void FindingWayByAStar(UFCNavigationGridBuilder* NavigationGridBuilder, FPointNavigationGrid Start, FPointNavigationGrid Target, OUT TArray<FPointNavigationGrid>& OutResultWay, OUT float& ResultTime);

private:
	static void DrawDebugInformationAStar(UFCNavigationGridBuilder* NavigationGridBuilder, TArray<FPointNavigationGrid> NavigationGridPoints, TMap<int32, float> CostSoFar, TMap<int32, int32> CameFrom, TArray<FPointNavigationGrid> ResultWay);

	static float Heuristic(FVector A, FVector B, float Alpha);
};
