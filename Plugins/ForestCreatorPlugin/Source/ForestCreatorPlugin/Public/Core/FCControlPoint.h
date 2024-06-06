

#pragma once

#include "CoreMinimal.h"
#include "ForestCreator.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "FCControlPoint.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API AFCControlPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFCControlPoint();

	void DrawDebugInformation(int32 Index);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRenderComponent;
};
