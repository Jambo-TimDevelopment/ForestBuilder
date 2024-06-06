

#pragma once

#include "CoreMinimal.h"
#include "FCBiomesHeightMapBuilder.h"
#include "FCBiomesPlacementObjectsBuilder.h"
#include "Engine/DataAsset.h"
#include "FCBiomesDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FORESTCREATORPLUGIN_API UFCBiomesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UFCBiomesHeightMapBuilder> BiomesHeightMapBuilder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BiomesSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UFCBiomesPlacementObjectsBuilder> PlacementObjectBuilder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor DebugColor;
};
