


#include "Core/ForestCreator.h"
#include "Core/FCFunctionLibrary.h"
#include "Core/FCBiomesMaskBuilder.h"
#include "Core/FCHeightMapSmoothingFilter.h"
#include "ForestCreatorUtils.h"
#include "Landscape.h"
#include "SAdvancedTransformInputBox.h"
#include "Character/FCAIController.h"
#include "Character/FCCharacter.h"
#include "Core/FCBiomesDataAsset.h"
#include "Core/FCBiomesPlacementObjectsBuilder.h"
#include "Core/FCControlPoint.h"
#include "Core/FCNavigationGridBuilder.h"
#include "Core/FCPlacementControlPointsBuilder.h"
#include "Core/FCRiverBuilder.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AForestCreator::AForestCreator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent* SceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "SceneRoot");
	SceneComponent->SetMobility(EComponentMobility::Static);
	RootComponent = SceneComponent;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
}

void AForestCreator::BuildForest()
{
	FMath::RandInit(Seed);
	FMath::SRandInit(Seed);
	FRandomStream Stream(Seed);
	UKismetMathLibrary::SeedRandomStream(Stream);
	ALandscape* LandscapeForUpdate = nullptr;
	if (AActor* FoundObject = UFCFunctionLibrary::FindObjectByClass(ALandscape::StaticClass()))
	{
		bLandscapeWacCreated = true;
		LandscapeForUpdate = StaticCast<ALandscape*>(FoundObject);
	}

	if (!IsValid(BiomesMaskBuilderClass))
	{
		UE_LOG(LogForestCreator, Warning, TEXT("AForestCreator::BuildForest() BiomesMaskBuilderClass is not valid"));
#if WITH_EDITOR
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("BiomesMaskBuilderClass class in ForestCreator is not valid! Select BiomesMaskBuilderClass and try again."));
#endif
		return;
	}

	if (BiomesSet.IsEmpty())
	{
		UE_LOG(LogForestCreator, Warning, TEXT("AForestCreator::BuildForest() BiomesSet is not valid"));
#if WITH_EDITOR
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("BiomesSet class in ForestCreator is not valid! Select BiomesSet and try again."));
#endif
		return;
	}

	FTransform LandscapeTransform = FTransform();
	LandscapeTransform.SetScale3D(SectionScale);

	LandscapeParameters.LandscapeTransform = LandscapeTransform;
	LandscapeParameters.SectionSize = SectionSize;
	LandscapeParameters.ComponentCountX = ComponentCountX;
	LandscapeParameters.ComponentCountY = ComponentCountY;
	LandscapeParameters.SectionsPerComponent = SectionsPerComponent;
	LandscapeParameters.BiomesSet = BiomesSet;
	LandscapeParameters.SectionScale = SectionScale;
	LandscapeParameters.LandscapeMaterial = LandscapeMaterial;
	LandscapeParameters.GlobalHeightScale = GlobalHeightScale;

	const int32 QuadsPerComponent = SectionSize * SectionsPerComponent;
	const int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = ComponentCountY * QuadsPerComponent + 1;

	TArray<uint8> BiomesMask;
	UFCBiomesMaskBuilder* BiomesMaskBuilder = GetMutableDefault<UFCBiomesMaskBuilder>(BiomesMaskBuilderClass);
	BiomesMaskBuilder->CreateBiomesMask(LandscapeParameters, BiomesMask);
	LandscapeParameters.BiomesMask = BiomesMask;
	uint8 CountBiomesIndex = 0;

	if (BiomesSet.IsEmpty()) { return; }

	TArray<uint16> BiomesHeightMap;
	for (const UFCBiomesDataAsset* BiomesDataAsset : BiomesSet)
	{
		if (IsValid(BiomesDataAsset))
		{
			if (UFCBiomesHeightMapBuilder* HeightMapBuilder = GetMutableDefault<UFCBiomesHeightMapBuilder>(
				BiomesDataAsset->BiomesHeightMapBuilder))
			{
				BiomesHeightMap.SetNum(SizeX * SizeY);
				HeightMapBuilder->CreateBiomesHeightMap(LandscapeParameters, BiomesMask, BiomesHeightMap, CountBiomesIndex);
				CountBiomesIndex++;
			}
		}
	}

	LandscapeParameters.HeightData = BiomesHeightMap;

	ALandscape* Landscape = UFCFunctionLibrary::CreateLandscape(LandscapeParameters, LandscapeForUpdate);

	if (IsValid(RiverClass))
	{
		UFCRiverBuilder* River = GetMutableDefault<UFCRiverBuilder>(RiverClass);
		River->CreateRiver(LandscapeParameters, Landscape);
	}

	UFCNavigationGridBuilder* NavigationGridBuilder = GetMutableDefault<UFCNavigationGridBuilder>(NavigationGridBuilderClass);
	NavigationGridBuilder->CreateNavigationGrid(LandscapeParameters);

	UFCPlacementControlPointsBuilder* ControlPointsBuilder = GetMutableDefault<UFCPlacementControlPointsBuilder>(ControlPointBuilderClass);
	ControlPointsBuilder->CreateControlPoint(NavigationGridBuilder, ControlPoints);

	float GlobalResultTime = 0;
	int32 CountControlIndex = 0;
	TArray<float> TimeBetweenPoints;
	for (int32 ControlPointIndex : ControlPointsBuilder->ControlPointIndexes)
	{
		TArray<FPointNavigationGrid> OutResultWay;
		float ResultTime;
		int32 StartIndex = ControlPointsBuilder->StartControlPointIndex;
		FPointNavigationGrid StartPoint;
		if (CountControlIndex)
		{
			StartIndex = CountControlIndex - 1;
			StartPoint = NavigationGridBuilder->NavigationGridPoints[ControlPointsBuilder->ControlPointIndexes[StartIndex]];
		}
		else
		{
			StartPoint = NavigationGridBuilder->NavigationGridPoints[StartIndex];
		}
		const int32 TargetIndex = ControlPointIndex;
		UFCFunctionLibrary::FindingWayByAStar(NavigationGridBuilder, StartPoint, NavigationGridBuilder->NavigationGridPoints[TargetIndex], OutResultWay, ResultTime);
		TimeBetweenPoints.Add(ResultTime);
		GlobalWay.Append(OutResultWay);
		GlobalResultTime += ResultTime;
		CountControlIndex++;
	}

	if (AFCAIController* AIController = StaticCast<AFCAIController*>(UGameplayStatics::GetActorOfClass(GetWorld(), AFCAIController::StaticClass())))
	{
		AIController->GlobalWay.Append(GlobalWay);
		AIController->PerspectiveResultTime = GlobalResultTime;
		AIController->PerspectiveTimeBetweenPoints = TimeBetweenPoints;

		AFCCharacter* Character = StaticCast<AFCCharacter*>(AIController->GetPawn());
		Character->DrawInfoAboutWay(GlobalResultTime);
	}

	if (bLandscapeWacCreated) { return; }
	int32 CurrentBiomesIndex = 0;
	for (UFCBiomesDataAsset* DataAsset : BiomesSet)
	{
		if (DataAsset->PlacementObjectBuilder)
		{
			UFCBiomesPlacementObjectsBuilder* PlacementObjectsBuilder = GetMutableDefault<UFCBiomesPlacementObjectsBuilder>(DataAsset->PlacementObjectBuilder);
			PlacementObjectsBuilder->CreateBiomesPlacementObjectsBuilder(LandscapeParameters, CurrentBiomesIndex);
		}
		CurrentBiomesIndex++;
	}
}

void AForestCreator::BeginPlay()
{
	Super::BeginPlay();
	BuildForest();
}

TSet<UFCBiomesDataAsset*> AForestCreator::GetBiomesSet()
{
	return BiomesSet;
}
