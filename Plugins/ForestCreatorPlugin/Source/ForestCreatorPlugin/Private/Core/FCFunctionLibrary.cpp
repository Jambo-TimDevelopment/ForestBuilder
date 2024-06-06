


#include "Core/FCFunctionLibrary.h"
#include "Landscape.h"
#include "LandscapeInfo.h"
#include "Animation/Rig.h"
#include "Core/FCBiomesDataAsset.h"
#include "Core/FCDebugNavigationPointActor.h"
#include "Core/ForestCreator.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Util/IndexPriorityQueue.h"

AActor* UFCFunctionLibrary::FindObjectByClass(TSubclassOf<AActor> ActorClass)
{
	if (const UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		return UGameplayStatics::GetActorOfClass(World, ActorClass);
	}

	return nullptr;
}

ALandscape* UFCFunctionLibrary::CreateLandscape(const FLandscapeParameters& Parameters, ALandscape* LandscapeForUpdate)
{
	int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;

	int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
	int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

	TArray<FLandscapeImportLayerInfo> MaterialImportLayers;
	MaterialImportLayers.Reserve(0);


	TMap<FGuid, TArray<uint16>> HeightDataPerLayers = Parameters.HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;

	TArray<uint16> HeightData = Parameters.HeightData;

	HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightData)); /*ENewLandscapePreviewMode.NewLandscape*/
	// ComputeHeightData will also modify/expand material layers data, which is why we create MaterialLayerDataPerLayers after calling ComputeHeightData
	MaterialLayerDataPerLayers.Add(FGuid(), MoveTemp(MaterialImportLayers));

	//FScopedTransaction Transaction(TEXT("Undo", "Creating New Landscape"));

	UWorld* World = nullptr;
	{
		// We want to create the landscape in the landscape editor mode's world
		FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
		World = EditorWorldContext.World();
	}

	ALandscape* Landscape;
	if (IsValid(LandscapeForUpdate))
	{
		Landscape = LandscapeForUpdate;
	}
	else
	{
		Landscape = World->SpawnActor<ALandscape>();
	}
	Landscape->bCanHaveLayersContent = false;
	Landscape->LandscapeMaterial = nullptr;

	if (Parameters.LandscapeMaterial)
	{
		Landscape->LandscapeMaterial = Parameters.LandscapeMaterial;
	}
	else
	{
		FSoftObjectPath MyAssetPath("Material'/Game/Material/Experemenrtal.Experemenrtal'");
		UObject* MyAsset = MyAssetPath.TryLoad();
		Landscape->LandscapeMaterial = (UMaterial*)MyAsset;
	}

	Landscape->SetActorTransform(Parameters.LandscapeTransform);

	// automatically calculate a lighting LOD that won't crash lightmass (hopefully)
	// < 2048x2048 -> LOD0
	// >=2048x2048 -> LOD1
	// >= 4096x4096 -> LOD2
	// >= 8192x8192 -> LOD3

	//const FGuid& InGuid, int32 InMinX, int32 InMinY, int32 InMaxX, int32 InMaxY, int32 InNumSubsections, int32 InSubsectionSizeQuads, const TMap<FGuid, TArray<uint16>>& InImportHeightData,
	//	const TCHAR* const InHeightmapFileName, const TMap<FGuid, TArray<FLandscapeImportLayerInfo>>& InImportMaterialLayerInfos, ELandscapeImportAlphamapType InImportMaterialLayerType, const TArray<struct FLandscapeLayer>* InImportLayers = nullptr

	Landscape->Import(FGuid::NewGuid(), 0, 0, SizeX - 1, SizeY - 1, Parameters.SectionsPerComponent, QuadsPerComponent, HeightDataPerLayers, nullptr, MaterialLayerDataPerLayers, ELandscapeImportAlphamapType::Additive);

	Landscape->StaticLightingLOD = FMath::DivideAndRoundUp(FMath::CeilLogTwo((SizeX * SizeY) / (2048 * 2048) + 1), (uint32)2);

	// Register all the landscape components
	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();

	LandscapeInfo->UpdateLayerInfoMap(Landscape);

	Landscape->RegisterAllComponents();

	// // Need to explicitly call PostEditChange on the LandscapeMaterial property or the landscape proxy won't update its material
	// FPropertyChangedEvent MaterialPropertyChangedEvent(
	// 	FindFieldChecked<FProperty>(Landscape->GetClass(), FName("LandscapeMaterial")));
	// Landscape->PostEditChangeProperty(MaterialPropertyChangedEvent);
	// Landscape->PostEditChange();

	return Landscape;
}

void UFCFunctionLibrary::UpdateOwningLandscape(ALandscapeBlueprintBrushBase* Brush, ALandscape* Landscape)
{
	Brush->SetOwningLandscape(Landscape);
}

void UFCFunctionLibrary::FindingWayByDijkstra(UFCNavigationGridBuilder* UfcNavigationGridBuilder, FPointNavigationGrid Start, FPointNavigationGrid End)
{
	/*FPointNavigationGrid StartPoint = Start;
	FPointNavigationGrid EndPoint = End;

   TArray<FPointNavigationGrid> NotVisited = UfcNavigationGridBuilder->NavigationGridPoints;
	// TODO our path point, prevElementIndex
   TArray<FPointNavigationGrid, int> Track;
   Track.Add(Start, NULL);
   while (true)
   {
	   FPointNavigationGrid* Open = nullptr;
	   float ShortestDistance = 10;
	   for(FPointNavigationGrid FPoint : NotVisited)
	   {
		   if(Track.Contains(FPoint) && Track.Find(FPoint) > ShortestDistance)
		   {
			   ShortestDistance = Track.Find(FPoint);
			   Open = &FPoint;
		   }
	   }
   
	   if (Open == nullptr) return;
	   if (Open == &EndPoint) break;
   
	   //TODO disclosure of vertices
	   for (int32 FPoint : UfcNavigationGridBuilder.NavigationGridPoints[Open->PointIndex].NeighborsPointIndex)
	   {
		   float CurrentShortDistance = Track[Open]. + weights[e];
		   var nextNode = e.OtherNode(toOpen);
		   if (!track.ContainsKey(nextNode) || track[nextNode].Price > currentPrice)
		   {
			   track[nextNode] = new DijkstraData { Previous = toOpen, Price = currentPrice };
		   }
	   }
   }*/
}

void UFCFunctionLibrary::FindingWayByAStar(UFCNavigationGridBuilder* NavigationGridBuilder, FPointNavigationGrid Start, FPointNavigationGrid Target, OUT TArray<FPointNavigationGrid>& OutResultWay, OUT float& ResultTime)
{
	if (!IsValid(NavigationGridBuilder)) { return; }

	UE::Geometry::FIndexPriorityQueue FrontierQueue = UE::Geometry::FIndexPriorityQueue(NavigationGridBuilder->NavigationGridPoints.Num());
	TArray<FPointNavigationGrid> NavigationGridPoints = NavigationGridBuilder->NavigationGridPoints;
	TMap<int32, float> CostSoFar;
	TMap<int32, int32> CameFrom;

	FrontierQueue.Insert(Target.PointIndex, 0.0f);
	CostSoFar.Add(Target.PointIndex, 0.0f);
	CameFrom.Add(Target.PointIndex, Target.PointIndex);

	while (FrontierQueue.GetCount())
	{
		const int32 CurrentPointIndex = FrontierQueue.GetFirstNodeID();
		FPointNavigationGrid CurrentPoint = NavigationGridPoints[CurrentPointIndex];

		TMap<int32, FPointNavigationGridSide> CurrentNeighborsPointIndex = CurrentPoint.NeighborsPointIndex;
		for (auto NextPoint : CurrentNeighborsPointIndex)
		{
			const float NewCost = CostSoFar[CurrentPointIndex] + NavigationGridPoints[CurrentPointIndex].NeighborsPointIndex[NextPoint.Key].Time;
			if (!CostSoFar.Contains(NextPoint.Key) || NewCost < CostSoFar[NextPoint.Key])
			{
				if (!CostSoFar.Contains(NextPoint.Key))
				{
					CostSoFar.Add(NextPoint.Key, NewCost);
				}
				else
				{
					CostSoFar[NextPoint.Key] = NewCost;
				}
				const float Priority = NewCost + Heuristic(Start.Location, NavigationGridPoints[NextPoint.Key].Location, NavigationGridBuilder->HeuristicAlpha); // NewCost;
				if (FrontierQueue.Contains(NextPoint.Key))
				{
					FrontierQueue.Update(NextPoint.Key, Priority);
				}
				else
				{
					FrontierQueue.Insert(NextPoint.Key, Priority);
				}
				if (!CameFrom.Contains(NextPoint.Key))
				{
					CameFrom.Add(NextPoint.Key, CurrentPointIndex);
				}
				else
				{
					CameFrom[NextPoint.Key] = CurrentPointIndex;
				}
			}
		}

		if (FrontierQueue.Contains(CurrentPointIndex))
		{
			FrontierQueue.Remove(CurrentPointIndex);
		}

		if (CurrentPointIndex == Start.PointIndex)
		{
			break;
		}
	}

	// TODO Get array with way
	TArray<FPointNavigationGrid> ResultWay;
	//for(int32 WayPoint : CameFrom.)
	int32 WayIndex = Start.PointIndex;
	while (WayIndex != Target.PointIndex)
	{
		ResultWay.Add(NavigationGridPoints[WayIndex]);
		WayIndex = CameFrom[WayIndex];
	}
	ResultWay.Add(NavigationGridPoints[Target.PointIndex]);

	DrawDebugInformationAStar(NavigationGridBuilder, NavigationGridPoints, CostSoFar, CameFrom, ResultWay);

	OutResultWay = ResultWay;
	ResultTime = CostSoFar[Start.PointIndex];
}

void UFCFunctionLibrary::DrawDebugInformationAStar(UFCNavigationGridBuilder* NavigationGridBuilder, TArray<FPointNavigationGrid> NavigationGridPoints, TMap<int32, float> CostSoFar, TMap<int32, int32> CameFrom, TArray<FPointNavigationGrid> ResultWay)
{
	if (NavigationGridBuilder->bEnableNavigationDebugData)
	{
		for (auto DebugPoint : CameFrom)
		{
			const int32 CurrentIndex = DebugPoint.Key;
			const int32 PrevIndex = DebugPoint.Value;
			FPointNavigationGrid CurrentPoint = NavigationGridPoints[CurrentIndex];

			AFCDebugNavigationPointActor* DebugPointActor = nullptr;
			FActorSpawnParameters SpawnParameters;
			FTransform DebugPointTransform;
			DebugPointTransform.SetLocation(CurrentPoint.Location + FVector(0, 0, 20000));

			DebugPointActor = NavigationGridBuilder->CachedForestCreator.Get()->GetWorld()->SpawnActor<AFCDebugNavigationPointActor>(NavigationGridBuilder->DebugNavigationPointActorClass, DebugPointTransform, SpawnParameters);

			FVector Direction = NavigationGridPoints[PrevIndex].Location - NavigationGridPoints[CurrentIndex].Location;
			FVector Location = NavigationGridPoints[PrevIndex].Location - Direction / 2 + FVector(0, 0, 20000);
			if (Direction != FVector(0))
			{
				DebugPointActor->DrawDebugPointDirection(Direction, Location);
			}

			FColor Color = NavigationGridBuilder->CachedForestCreator->GetBiomesSet().Array()[NavigationGridPoints[CurrentIndex].BiomesIndex]->DebugColor.ToFColor(false);
			DebugPointActor->DrawDebugCostSoFar(CostSoFar[CurrentIndex], Color, CurrentIndex);
		}

		for (FPointNavigationGrid WayPoint : ResultWay)
		{
			AFCDebugNavigationPointActor* DebugPointActor = nullptr;
			FActorSpawnParameters SpawnParameters;
			FTransform DebugPointTransform;
			DebugPointTransform.SetLocation(WayPoint.Location + FVector(0, 0, 30000));

			DebugPointActor = NavigationGridBuilder->CachedForestCreator.Get()->GetWorld()->SpawnActor<AFCDebugNavigationPointActor>(NavigationGridBuilder->DebugNavigationPointActorClass, DebugPointTransform, SpawnParameters);

			const int32 PrevIndex = CameFrom[WayPoint.PointIndex];
			if (PrevIndex >= 0)
			{
				FVector Direction = NavigationGridPoints[PrevIndex].Location - NavigationGridPoints[WayPoint.PointIndex].Location;
				FVector Location = NavigationGridPoints[PrevIndex].Location - Direction / 2 + FVector(0, 0, 20000);
				if (Direction != FVector(0))
				{
					DebugPointActor->DrawDebugPointDirection(Direction, Location);
				}
			}

			FColor Color = NavigationGridBuilder->CachedForestCreator->GetBiomesSet().Array()[NavigationGridPoints[WayPoint.PointIndex].BiomesIndex]->DebugColor.ToFColor(false);
			DebugPointActor->DrawDebugCostSoFar(CostSoFar[WayPoint.PointIndex], Color, WayPoint.PointIndex, FColor::Silver);
		}
	}
}

float UFCFunctionLibrary::Heuristic(FVector A, FVector B, float Alpha)
{
	return (FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y) + FMath::Abs(A.Z - B.Z)) * Alpha;
}
