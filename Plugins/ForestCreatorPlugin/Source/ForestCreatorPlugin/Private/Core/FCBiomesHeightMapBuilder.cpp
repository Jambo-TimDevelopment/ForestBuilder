


#include "Core/FCBiomesHeightMapBuilder.h"

void UFCBiomesHeightMapBuilder::CreateBiomesHeightMap(const FLandscapeParameters& Parameters, const TArray<uint8>& BiomesMask, OUT TArray<uint16>& BiomesHeightMap, uint8 BiomesIndex)
{
	if (BiomesHeightMap.IsEmpty())
	{
		const int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;
		const int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
		const int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

		BiomesHeightMap.SetNum(SizeX * SizeY);
	}

	for (int I = 0; I < BiomesMask.Num(); ++I)
	{
		if (BiomesMask[I] == BiomesIndex)
		{
			BiomesHeightMap[I] = BiomesIndex * 300;
		}
	}
}
