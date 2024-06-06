


#include "Core/FCPlacementControlPointsBuilder.h"
#include "Landscape.h"
#include "Core/FCControlPoint.h"
#include "Core/FCNavigationGridBuilder.h"

void UFCPlacementControlPointsBuilder::CreateControlPoint(UFCNavigationGridBuilder* NavigationGridBuilder, TMap<int32, AFCControlPoint*> NumberControlPoints)
{
	int32 CountControlPoints = ControlPointIndexes.Num() - 1;
	for (auto ControlPointIndex : ControlPointIndexes)
	{
		UWorld* World = nullptr;
		{
			// We want to create the landscape in the landscape editor mode's world
			FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
			World = EditorWorldContext.World();
		}
		TArray<FPointNavigationGrid> NavigationGridPoints = NavigationGridBuilder->NavigationGridPoints;

		AFCControlPoint* ControlPoint = GetMutableDefault<AFCControlPoint>();
		FActorSpawnParameters SpawnParameters;
		ControlPoint = World->SpawnActor<AFCControlPoint>(ControlPointClass, SpawnParameters);
		ControlPoint->DrawDebugInformation(CountControlPoints);
		ControlPoint->SetActorLocation(NavigationGridPoints[ControlPointIndex].Location, false, nullptr, ETeleportType::None);
		NumberControlPoints.Add(CountControlPoints, ControlPoint);
		CountControlPoints--;
	}
}
