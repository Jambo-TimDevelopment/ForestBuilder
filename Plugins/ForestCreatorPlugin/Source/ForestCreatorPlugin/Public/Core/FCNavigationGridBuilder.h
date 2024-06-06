

#pragma once

#include "CoreMinimal.h"
struct FPointNavigationGridSide;
class AFCDebugNavigationPointActor;
class AForestCreator;
struct FLandscapeParameters;
#include "UObject/NoExportTypes.h"
#include "FCNavigationGridBuilder.generated.h"

/**
 * 
 */

UENUM(Blueprintable, BlueprintType)
enum ENavigationPointSide
{
	None,
	Up,
	UpRight,
	Right,
	DownRight,
	Down,
	DownLeft,
	Left,
	UpLeft
};

USTRUCT(Blueprintable, BlueprintType)
struct FPointNavigationGridSide
{
	GENERATED_BODY()
public:
	FPointNavigationGridSide()
	{
		Side = None;
		Time = -1;
	};

	FPointNavigationGridSide(float Time, ENavigationPointSide Side)
	{
		this->Time = Time;
		this->Side = Side;
	}

	float Time;
	ENavigationPointSide Side;
};

USTRUCT(BlueprintType)
struct FPointNavigationGrid
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	int32 BiomesIndex;

	UPROPERTY(EditAnywhere)
	int32 PointIndex;

	UPROPERTY(EditAnywhere)
	int32 Speed;

	int32 PointPrevious = -1;

	bool bIsVisited = false;

	UPROPERTY(EditAnywhere)
	TMap<int32, FPointNavigationGridSide> NeighborsPointIndex;

	UPROPERTY()
	AFCDebugNavigationPointActor* DebugNavigationPointActor;
};


UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCNavigationGridBuilder : public UObject
{
	GENERATED_BODY()
public:
	void CreateNavigationGrid(FLandscapeParameters LandscapeParameters);
	void DrawDebugData();

	UPROPERTY(EditAnywhere)
	int32 NavigationGrid_SizeX = 10;

	UPROPERTY(EditAnywhere)
	int32 NavigationGrid_SizeY = 10;

	//UPROPERTY()
	TArray<FPointNavigationGrid> NavigationGridPoints;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFCDebugNavigationPointActor> DebugNavigationPointActorClass;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool bEnableNavigationDebugData = true;

	UPROPERTY(EditAnywhere, Category="Find way")
	float HeuristicAlpha;

	TWeakObjectPtr<AForestCreator> CachedForestCreator;
	float GetTimeBetweenPoints(const FPointNavigationGrid& PointStart, const FPointNavigationGrid& PointEnd);
};
