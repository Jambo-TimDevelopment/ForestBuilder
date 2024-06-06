

#pragma once

#include "CoreMinimal.h"
#include "FCBiomesPlacementObjectsBuilder.generated.h"

struct FLandscapeParameters;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCBiomesPlacementObjectsBuilder : public UObject
{
	GENERATED_BODY()

public:
	virtual void CreateBiomesPlacementObjectsBuilder(const FLandscapeParameters& Parameters, uint8 CurrentBiomesIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ObjectDensity = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxPlacementObjectScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinPlacementObjectScale;

	UPROPERTY(EditDefaultsOnly)
	TSet<UStaticMesh*> SetOfPlacementObject;
};
