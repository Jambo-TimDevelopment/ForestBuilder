

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "FCDebugNavigationPointActor.generated.h"

struct FPointNavigationGrid;
struct FPointNavigationGridSide;

UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API AFCDebugNavigationPointActor : public AActor
{
	GENERATED_BODY()

public:
	AFCDebugNavigationPointActor();

	void DrawDebugPointData(FLinearColor BiomesDebugColor, const FPointNavigationGrid& NavigationPoint);

	void DrawDebugPointDirection(FVector Direction, FVector Location);

	void DrawDebugCostSoFar(float SoFar, FColor ColorText, int32 Index, FColor ColorPoint = FColor(0, 0, 0));

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToUPText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToDownText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToLeftText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToRightText;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToUpLeftText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToDownLeftText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToDownRightText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* LenghtToUpRightText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* IndexOfNavigationPointText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTextRenderComponent* SoFarTimeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* CameFromDirectionArrow;

	FLinearColor DebugColor;
};
