


#include "Core/FCDebugNavigationPointActor.h"

#include "Core/FCNavigationGridBuilder.h"
#include "Materials/MaterialInstanceDynamic.h"

AFCDebugNavigationPointActor::AFCDebugNavigationPointActor()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	SetRootComponent(StaticMeshComponent);

	LenghtToUPText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToUPText");
	LenghtToUPText->SetupAttachment(RootComponent);

	LenghtToDownText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToDownText");
	LenghtToDownText->SetupAttachment(RootComponent);

	LenghtToLeftText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToLeftText");
	LenghtToLeftText->SetupAttachment(RootComponent);

	LenghtToRightText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToRightText");
	LenghtToRightText->SetupAttachment(RootComponent);

	LenghtToUpLeftText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToUpLeftText");
	LenghtToUpLeftText->SetupAttachment(RootComponent);

	LenghtToDownLeftText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToDownLeftText");
	LenghtToDownLeftText->SetupAttachment(RootComponent);

	LenghtToDownRightText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToDownRightText");
	LenghtToDownRightText->SetupAttachment(RootComponent);

	LenghtToUpRightText = CreateDefaultSubobject<UTextRenderComponent>("LenghtToUpRightText");
	LenghtToUpRightText->SetupAttachment(RootComponent);

	SoFarTimeText = CreateDefaultSubobject<UTextRenderComponent>("SoFarTimeText");
	SoFarTimeText->SetupAttachment(RootComponent);

	IndexOfNavigationPointText = CreateDefaultSubobject<UTextRenderComponent>("IndexOfNavigationPointText");
	IndexOfNavigationPointText->SetupAttachment(RootComponent);

	CameFromDirectionArrow = CreateDefaultSubobject<UArrowComponent>("CameFromDirectionArrow");
	CameFromDirectionArrow->SetupAttachment(RootComponent);
}

void AFCDebugNavigationPointActor::DrawDebugPointData(FLinearColor BiomesDebugColor, const FPointNavigationGrid& NavigationPoint)
{
	this->DebugColor = BiomesDebugColor;
	StaticMeshComponent->SetVectorParameterValueOnMaterials("Color", FVector(BiomesDebugColor.R, BiomesDebugColor.G, BiomesDebugColor.B));

	for (const auto Neighbor : NavigationPoint.NeighborsPointIndex)
	{
		if (Neighbor.Value.Side == ENavigationPointSide::Up)
		{
			LenghtToUPText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 4)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::Down)
		{
			LenghtToDownText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::Right)
		{
			LenghtToRightText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::Left)
		{
			LenghtToLeftText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::UpLeft)
		{
			LenghtToUpLeftText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::DownLeft)
		{
			LenghtToDownLeftText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::DownRight)
		{
			LenghtToDownRightText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}
		if (Neighbor.Value.Side == ENavigationPointSide::UpRight)
		{
			LenghtToUpRightText->SetText(FText::FromString(FString::SanitizeFloat(Neighbor.Value.Time, 0)));
		}

		IndexOfNavigationPointText->SetText(FText::FromString(FString::SanitizeFloat(NavigationPoint.PointIndex, 0)));
	}
}

void AFCDebugNavigationPointActor::DrawDebugPointDirection(FVector Direction, FVector Location)
{
	CameFromDirectionArrow->SetVisibility(true);
	CameFromDirectionArrow->SetWorldRotation(Direction.Rotation());
	CameFromDirectionArrow->SetWorldLocation(Location);
}

void AFCDebugNavigationPointActor::DrawDebugCostSoFar(float SoFar, FColor ColorText, int32 Index, FColor ColorPoint)
{
	SoFarTimeText->SetText(FText::FromString(FString::SanitizeFloat(SoFar, 2)));
	SoFarTimeText->SetTextRenderColor(ColorText);

	IndexOfNavigationPointText->SetText(FText::FromString(FString::SanitizeFloat(Index, 0)));
	this->DebugColor = ColorPoint;
	StaticMeshComponent->SetVectorParameterValueOnMaterials("Color", FVector(ColorPoint.R, ColorPoint.G, ColorPoint.B));

}
