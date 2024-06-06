


#include "Core/FCNavigationGridBuilder.h"
#include "Core/FCBiomesDataAsset.h"
#include "Core/ForestCreator.h"
#include "DrawDebugHelpers.h"
#include "Core/FCDebugNavigationPointActor.h"
#include "Core/FCFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UFCNavigationGridBuilder::CreateNavigationGrid(FLandscapeParameters LandscapeParameters)
{
	const int32 QuadsPerComponent = LandscapeParameters.SectionSize * LandscapeParameters.SectionsPerComponent;
	const int32 LandscapeSizeX = LandscapeParameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 LandscapeSizeY = LandscapeParameters.ComponentCountY * QuadsPerComponent + 1;

	const int32 NavigationGridSize = NavigationGrid_SizeX * NavigationGrid_SizeY;

	// We want to create the landscape in the landscape editor mode's world
	const FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
	const UWorld* World = EditorWorldContext.World();

	if (!CachedForestCreator.IsValid())
	{
		CachedForestCreator = StaticCast<AForestCreator*>(UGameplayStatics::GetActorOfClass(World, AForestCreator::StaticClass()));
		if (!CachedForestCreator.IsValid())
		{
			return;
		}
	}

	for (int32 NavigationGridIndex = 0; NavigationGridIndex < NavigationGridSize; NavigationGridIndex++)
	{
		const int32 I_NavGrid = NavigationGridIndex % NavigationGrid_SizeX;
		const int32 J_NavGrid = NavigationGridIndex / NavigationGrid_SizeY;

		const int32 I_Biomes = I_NavGrid * (LandscapeSizeX / static_cast<float>(NavigationGrid_SizeX));
		const int32 J_Biomes = J_NavGrid * (LandscapeSizeY / static_cast<float>(NavigationGrid_SizeY));

		const float X_PositionNavPointOnLandscape = I_Biomes * LandscapeParameters.SectionScale.X + LandscapeParameters.SectionScale.X / 2 * (LandscapeSizeX / static_cast<float>(NavigationGrid_SizeX));
		const float Y_PositionNavPointOnLandscape = J_Biomes * LandscapeParameters.SectionScale.Y + LandscapeParameters.SectionScale.Y / 2 * (LandscapeSizeY / static_cast<float>(NavigationGrid_SizeY));

		float Z = -10000;
		const FCollisionQueryParams CollisionQueryParams;
		const FCollisionResponseParams CollisionResponseParams;
		FHitResult Hit;
		FVector Start(X_PositionNavPointOnLandscape, Y_PositionNavPointOnLandscape, 0.0f);
		FVector End(X_PositionNavPointOnLandscape, Y_PositionNavPointOnLandscape, -10000000.0f);
		FName ProfileCollision = FName("LandscapeCollisionProfile");
		if (World->LineTraceSingleByProfile(Hit, Start, End, ProfileCollision, CollisionQueryParams))
		{
			Z = Hit.Location.Z;
		}

		const FVector GridPointLocation = FVector(X_PositionNavPointOnLandscape, Y_PositionNavPointOnLandscape, Z);
		FPointNavigationGrid NavGridPoint;
		NavGridPoint.Location = GridPointLocation;

		const int32 I_NavGridPointOnBiomesMask = GridPointLocation.X / LandscapeParameters.SectionScale.X;
		const int32 J_NavGridPointOnBiomesMask = GridPointLocation.Y / LandscapeParameters.SectionScale.Y;
		const int32 i_NavGridPointOnBiomesMask = J_NavGridPointOnBiomesMask * LandscapeSizeY + I_NavGridPointOnBiomesMask;

		if (LandscapeParameters.BiomesMask.Num() <= i_NavGridPointOnBiomesMask || i_NavGridPointOnBiomesMask < 0)
		{
			return;
		}

		const int32 BiomesIndexForNavPoint = LandscapeParameters.BiomesMask[i_NavGridPointOnBiomesMask];
		NavGridPoint.BiomesIndex = BiomesIndexForNavPoint;
		NavGridPoint.PointIndex = NavigationGridIndex;
		TArray<UFCBiomesDataAsset*> Biomes = CachedForestCreator->GetBiomesSet().Array();
		const float BiomesSpeed = Biomes[BiomesIndexForNavPoint]->BiomesSpeed;
		NavGridPoint.Speed = BiomesSpeed;
		NavigationGridPoints.Add(NavGridPoint);
	}

	for (int32 NavigationGridIndex = 0; NavigationGridIndex < NavigationGridSize; NavigationGridIndex++)
	{
		// TODO check Up left point
		if (NavigationGridIndex % NavigationGrid_SizeX > 0 && NavigationGridIndex / NavigationGrid_SizeX - 1 >= 0)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex - NavigationGrid_SizeX - 1]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex - NavigationGrid_SizeX - 1, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::UpLeft));
		}
		// TODO check Up point
		if (NavigationGridIndex - NavigationGrid_SizeX >= 0)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex - NavigationGrid_SizeX]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex - NavigationGrid_SizeX, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::Up));
		}
		// TODO check Up right point
		if (NavigationGridIndex - NavigationGrid_SizeX >= 0 && NavigationGridIndex % NavigationGrid_SizeY != NavigationGrid_SizeY - 1)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex - NavigationGrid_SizeX + 1]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex - NavigationGrid_SizeX + 1, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::UpRight));
		}
		// TODO check right point
		if (NavigationGridIndex % NavigationGrid_SizeY != NavigationGrid_SizeY - 1)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex + 1]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex + 1, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::Right));
		}
		// TODO check down right point
		if (NavigationGridIndex + NavigationGrid_SizeX < NavigationGridSize && NavigationGridIndex % NavigationGrid_SizeY != NavigationGrid_SizeY - 1)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex + NavigationGrid_SizeX + 1]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex + NavigationGrid_SizeX + 1, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::DownRight));
		}
		// TODO check down point
		if (NavigationGridIndex + NavigationGrid_SizeX < NavigationGridSize)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex + NavigationGrid_SizeX]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex + NavigationGrid_SizeX, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::Down));
		}
		// TODO check down left point
		if (NavigationGridIndex + NavigationGrid_SizeX < NavigationGridSize && NavigationGridIndex % NavigationGrid_SizeX > 0)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex + NavigationGrid_SizeX - 1]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex + NavigationGrid_SizeX - 1, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::DownLeft));
		}
		// TODO check left point
		if (NavigationGridIndex % NavigationGrid_SizeX > 0)
		{
			float TimeBetweenPoints = GetTimeBetweenPoints(NavigationGridPoints[NavigationGridIndex], NavigationGridPoints[NavigationGridIndex - 1]);
			NavigationGridPoints[NavigationGridIndex].NeighborsPointIndex.Add(NavigationGridIndex - 1, FPointNavigationGridSide(TimeBetweenPoints, ENavigationPointSide::Left));
		}
	}

	if (bEnableNavigationDebugData)
	{
		DrawDebugData();
	}

	// Test
}

void UFCNavigationGridBuilder::DrawDebugData()
{
	UWorld* World = nullptr;
	// We want to create the landscape in the landscape editor mode's world
	FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
	World = EditorWorldContext.World();

	for (FPointNavigationGrid Point : NavigationGridPoints)
	{
		const FVector DebugPointLocation = Point.Location + FVector(0, 0, 10000);
		for (auto NeighborsPointInd : Point.NeighborsPointIndex)
		{
			DrawDebugLine(World, DebugPointLocation, NavigationGridPoints[NeighborsPointInd.Key].Location + FVector(0, 0, 10000), FColor(225, 0, 25), true, 1, 0, 100);
		}
		DrawDebugSphere(World, Point.Location, 100, 8, FColor(225, 0, 25), true, 1, 0, 100);

		FActorSpawnParameters SpawnParameters;
		FTransform DebugPointTransform;
		DebugPointTransform.SetLocation(DebugPointLocation);

		AFCDebugNavigationPointActor* DebugPoint = World->SpawnActor<AFCDebugNavigationPointActor>(DebugNavigationPointActorClass, DebugPointTransform, SpawnParameters);
		Point.DebugNavigationPointActor = DebugPoint;
		DebugPoint->DrawDebugPointData(CachedForestCreator->GetBiomesSet().Array()[Point.BiomesIndex]->DebugColor, Point);
	}
}

float UFCNavigationGridBuilder::GetTimeBetweenPoints(const FPointNavigationGrid& PointStart, const FPointNavigationGrid& PointEnd)
{
	const float SpeedBetweenPoints = (PointStart.Speed + PointEnd.Speed) / 2;
	const float Length = (PointStart.Location - PointEnd.Location).Length();
	const float Time = Length / SpeedBetweenPoints;

	return Time;
}
