


#include "Tests/FCActorsLanscapeTest.h"
#include "CoreMinimal.h"
#include "Core/FCControlPoint.h"
#include "Engine/StaticMeshActor.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine/World.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorControlPointCanBeCreated, "RunForestVR.Items.Biomes.FCppActorControlPointCanBeCreated",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionShouldBeSetubCorrectly, "RunForestVR.Items.Biomes.FCollisionShouldBeSetubCorrectly",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorBiomesCanBeCreated, "RunForestVR.Items.Biomes.FCppActorCanBeCreated",
								 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
								 EAutomationTestFlags::HighPriority);

namespace
{
	UWorld* GetTestGameWorld()
	{
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for (const FWorldContext& Context : WorldContexts)
		{
			if ((Context.WorldType == EWorldType::PIE || (Context.WorldType == EWorldType::Game) && Context.World()))
			{
				return Context.World();
			}
		}

		return nullptr;
	}
}

class LevelScope
{
public:
	LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }

	~LevelScope()
	{
		ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
	}
};

bool FCppActorControlPointCanBeCreated::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/TestLevel.TestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World point exist", World))
	{
		return false;
	}
	const FTransform FTransform{FVector{1000.f}};
	const AFCControlPoint* ControlPoint = World->SpawnActor<AFCControlPoint>(AFCControlPoint::StaticClass(), FTransform);
	if (!TestNotNull("Control point exist", ControlPoint))
	{
		return false;
	}
	return true;
}

bool FCollisionShouldBeSetubCorrectly::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/TestLevel.TestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World point exist", World))
	{
		return false;
	}
	const FTransform FTransform{FVector{1000.f}};
	const AFCControlPoint* ControlPoint = World->SpawnActor<AFCControlPoint>(AFCControlPoint::StaticClass(), FTransform);
	if (!TestNotNull("Control point exist", ControlPoint))
	{
		return false;
	}
	const auto CollisionComp = ControlPoint->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Cube Component exist", CollisionComp))
	{
		return false;
	}

	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	TestTrueExpr(CollisionComp->GetAttachmentRoot() == CollisionComp);
	return true;
}

bool FCppActorBiomesCanBeCreated::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/TestLevel.TestLevel");

	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World point exist", World))
	{
		return false;
	}
	
	TSet<UStaticMesh*> SetOfPlacementObject;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStaticMeshActor* PlacementObject = World->SpawnActor<AStaticMeshActor>(SpawnParameters);
	PlacementObject->SetActorLocation(FVector(1000,1000,1000));
	PlacementObject->GetStaticMeshComponent()->SetBoundsScale(5);
	PlacementObject->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
	if (!TestNotNull("PlacementObject exist", PlacementObject))
	{
		return false;
	}
	return true;
}
