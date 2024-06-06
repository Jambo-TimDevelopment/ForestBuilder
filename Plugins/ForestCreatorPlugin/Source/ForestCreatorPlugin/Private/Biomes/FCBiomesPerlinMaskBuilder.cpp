


#include "Biomes/FCBiomesPerlinMaskBuilder.h"
#include "ForestCreatorUtils.h"
#include "Core/FCDeveloperSettings.h"
#include "Core/ForestCreator.h"
#include "Kismet/KismetMathLibrary.h"

void UFCBiomesPerlinMaskBuilder::CreateBiomesMask(FLandscapeParameters& Parameters, TArray<uint8>& BiomesMask)
{
	const int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;
	const int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

	BiomesMask.SetNum(SizeX * SizeY);
	Parameters.GlobalHeightData.SetNum(SizeX * SizeY);

	const UFCDeveloperSettings* Settings = GetDefault<UFCDeveloperSettings>();
	const FRandomStream Stream(Settings->Seed);
	const float SeedOffset = UKismetMathLibrary::RandomFloatInRangeFromStream(0, 10000, Stream);

	for (int32 i = 0; i < BiomesMask.Num(); i++)
	{
		const uint16 I = i % SizeX;
		const uint16 J = i / SizeY;

		const FVector2d PerlinPosition = FVector2d(I * PerlinNoiseScale + SeedOffset, J * PerlinNoiseScale + SeedOffset);
		const float RawHeight = (FMath::PerlinNoise2D(PerlinPosition) + 1) / 2 * (Parameters.BiomesSet.Num() + 1) - 1;

		BiomesMask[i] = RawHeight;
		Parameters.GlobalHeightData[i] = RawHeight;

		//UE_LOG(LogForestCreator, Warning, TEXT("I=%d, J=%d, SizeX=%d, SizeY=%d, BiomesMask[i]=%f"), I, J, SizeX, SizeY, RawHeight);
	}
}
