


#include "Core/FCBiomesMaskBuilder.h"
#include "Core/ForestCreator.h"

void UFCBiomesMaskBuilder::CreateBiomesMask(FLandscapeParameters& Parameters, OUT TArray<uint8>& BiomesMask)
{
	const int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;
	const int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

	BiomesMask.SetNum(SizeX * SizeY);
	Parameters.GlobalHeightData.SetNum(SizeX * SizeY);

	for (int32 i = 0; i < BiomesMask.Num(); i++)
	{
		int32 I = i / SizeX;
		int32 J = i % SizeY;

		BiomesMask[i] = FMath::RandRange(0, Parameters.BiomesSet.Num());
	}
}
