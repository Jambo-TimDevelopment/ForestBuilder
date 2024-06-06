


#include "Core/FCBiomesPlacementObjectsBuilder.h"

#include "Core/ForestCreator.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

void UFCBiomesPlacementObjectsBuilder::CreateBiomesPlacementObjectsBuilder(
	const FLandscapeParameters& Parameters, uint8 CurrentBiomesIndex)
{
	for (int32 i = 0; i < ObjectDensity; i++)
	{
		const int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;
		const int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
		const int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

		int32 X = FMath::RandRange(1, SizeX * (int32)Parameters.SectionScale.X - 1);
		int32 Y = FMath::RandRange(1, SizeY * (int32)Parameters.SectionScale.Y - 1);

		int32 I = X / Parameters.SectionScale.X;
		int32 J = Y / Parameters.SectionScale.Y;
		int32 Index = J * SizeY + I;
		if (CurrentBiomesIndex != Parameters.BiomesMask[Index])
		{
			continue;
		}

		FHitResult Hit;
		const FCollisionQueryParams CollisionQueryParams;
		const FCollisionResponseParams CollisionResponseParams;
		FVector Start(X, Y, 1000000);
		FVector End(X, Y, -100000000);
		int32 Z = 0;
		UWorld* World = nullptr;
		{
			// We want to create the landscape in the landscape editor mode's world
			FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
			World = EditorWorldContext.World();
		}
		if (World->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldDynamic, CollisionQueryParams,CollisionResponseParams))
		{
			Z = Hit.Location.Z;
			if (Cast<AStaticMeshActor>(Hit.GetActor()))
			{
				continue;
			}
		}
		if(Z == 0)
		{
			continue;
		}
		int32 IndexOfObjectPlacement = FMath::RandRange(0, SetOfPlacementObject.Num() - 1);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AStaticMeshActor* PlacementObject = World->SpawnActor<AStaticMeshActor>(SpawnParameters);
		PlacementObject->SetActorLocation(FVector(X, Y, Z));
		PlacementObject->GetStaticMeshComponent()->SetBoundsScale(5);
		PlacementObject->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		PlacementObject->GetStaticMeshComponent()->SetStaticMesh(SetOfPlacementObject.Array()[IndexOfObjectPlacement]);
	}
}
