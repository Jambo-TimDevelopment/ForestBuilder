


#include "Core/FCRiverBuilder.h"
#include "Components/SplineComponent.h"
#include "Core/FCFunctionLibrary.h"

void UFCRiverBuilder::CreateRiver(const FLandscapeParameters& LandscapeParameters, ALandscape* Landscape)
{
	UWorld* World = nullptr;
	{
		// We want to create the landscape in the landscape editor mode's world
		FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
		World = EditorWorldContext.World();
	}

	AActor* River = World->SpawnActor<AActor>(RiverBrushClass);
//	UFCFunctionLibrary::UpdateOwningLandscape(StaticCast<ALandscapeBlueprintBrushBase*>(River), Landscape);
	if (USplineComponent* RiverSpline = StaticCast<USplineComponent*>(River->GetComponentByClass(USplineComponent::StaticClass())))
	{
		TArray<FSplinePoint> Points;
		CreateRiverPoint(LandscapeParameters, Points);
		for (FSplinePoint point : Points)
		{
			RiverSpline->AddSplineWorldPoint(point.Position);
		}
		//RiverSpline->AddPoints(Points);
	}
}

void UFCRiverBuilder::CreateRiverPoint(const FLandscapeParameters& LandscapeParameters, TArray<FSplinePoint>& SplinePoints)
{
	const int32 QuadsPerComponent = LandscapeParameters.SectionSize * LandscapeParameters.SectionsPerComponent;
	const int32 SizeX = LandscapeParameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = LandscapeParameters.ComponentCountY * QuadsPerComponent + 1;

	SplinePoints.SetNum(CountSplinePointsInRiver);
	for (int i = 0; i < CountSplinePointsInRiver; ++i)
	{
		int32 X = FMath::RandRange(0, SizeX * 100);
		int32 Y = FMath::RandRange(0, SizeY * 100);

		FHitResult Hit;
		const FCollisionQueryParams CollisionQueryParams;
		const FCollisionResponseParams CollisionResponseParams;
		FVector Start(X, Y, 1000000);
		FVector End(X, Y, -1000000);
		int32 Z = 0;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldDynamic, CollisionQueryParams, CollisionResponseParams))
		{
			Z = Hit.Location.Z;
		}

		FSplinePoint SplinePoint;
		SplinePoint.Position = FVector(X, Y, Z);
		FRotator Rotation = i != 0 ? (SplinePoints.Last().Position - SplinePoint.Position).Rotation() : FRotator(0);
		SplinePoint.Rotation = Rotation;

		SplinePoints.Add(SplinePoint);
	}
}
