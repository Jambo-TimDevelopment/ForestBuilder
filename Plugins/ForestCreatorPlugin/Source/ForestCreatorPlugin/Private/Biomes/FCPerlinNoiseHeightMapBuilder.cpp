


#include "Biomes/FCPerlinNoiseHeightMapBuilder.h"
#include "ForestCreatorUtils.h"
#include "Core/FCDeveloperSettings.h"
#include "Kismet/KismetMathLibrary.h"

void UFCPerlinNoiseHeightMapBuilder::CreateBiomesHeightMap(const FLandscapeParameters& Parameters, const TArray<uint8>& BiomesMask,OUT TArray<uint16>& BiomesHeightMap, uint8 CurrentBiomesIndex)
{
	const int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;
	const int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

	if (BiomesHeightMap.IsEmpty())
	{
		BiomesHeightMap.SetNum(SizeX * SizeY);
	}

	for (int i = 0; i < BiomesMask.Num(); ++i)
	{
		const uint16 I = i % SizeX;
		const uint16 J = i / SizeY;

		if (BiomesMask[i] == CurrentBiomesIndex)
		{
			float Height = GetHeightByOctaves(I, J);
			BiomesHeightMap[i] = Height + FMath::Pow(Parameters.GlobalHeightData[i] + 1, Parameters.GlobalHeightScale);
			//UE_LOG(LogForestCreator, Warning, TEXT("CurrentBiomesIndex=%d, BiomesHeightMap[i]=%f"), CurrentBiomesIndex, BiomesHeightMap[i]);
		}
	}
}

float UFCPerlinNoiseHeightMapBuilder::GetHeightByOctaves(int X, int Y)
{
	const UFCDeveloperSettings* Settings = GetDefault<UFCDeveloperSettings>();
	FRandomStream Stream(Settings->Seed);
	float SeedOffset = UKismetMathLibrary::RandomFloatInRangeFromStream(0, 10000000, Stream);

	float ResultHeight = 0.0f;
	for (int i = 0; i < Octaves.Num(); i++)
	{
		const FVector2d PerlinPosition = FVector2d(X * Octaves[i].PerlinNoiseScale + SeedOffset, Y * Octaves[i].PerlinNoiseScale + SeedOffset);
		ResultHeight += (FMath::PerlinNoise2D(PerlinPosition) + 1) / 2 * Octaves[i].HeightScale;
	}

	return ResultHeight;
}
