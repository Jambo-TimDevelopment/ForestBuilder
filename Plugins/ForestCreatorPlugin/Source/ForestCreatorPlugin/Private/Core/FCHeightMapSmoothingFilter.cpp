


#include "Core/FCHeightMapSmoothingFilter.h"

void UFCHeightMapSmoothingFilter::ApplyHeightMapSmoothingFilter(const FLandscapeParameters& Parameters, TArray<uint16>& BiomesHeightMap)
{
	const int32 QuadsPerComponent = Parameters.SectionSize * Parameters.SectionsPerComponent;
	const int32 SizeX = Parameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = Parameters.ComponentCountY * QuadsPerComponent + 1;

	for (int i = 0; i < BiomesHeightMap.Num(); ++i)
	{
		int32 I = i / SizeX;
		int32 J = i % SizeY;

		if (I - 1 > 0 && J - 1 > 0 && I + 1 < SizeX && J + 1 < SizeY &&
			(
				FMath::Abs(BiomesHeightMap[(I - 1) * SizeX + J] - BiomesHeightMap[i]) > MinStepForApplyFilter ||
				FMath::Abs(BiomesHeightMap[(J - 1) + I * SizeY] - BiomesHeightMap[i]) > MinStepForApplyFilter ||
				FMath::Abs(BiomesHeightMap[(I + 1) * SizeX + J] - BiomesHeightMap[i]) > MinStepForApplyFilter ||
				FMath::Abs(BiomesHeightMap[(J + 1) + I * SizeY] - BiomesHeightMap[i]) > MinStepForApplyFilter
			)
		)
		{
			const float SmoothHeight1 = FMath::CubicInterp(static_cast<float>(BiomesHeightMap[(I - 1) * SizeX + J]), 0.0f, static_cast<float>(BiomesHeightMap[i]), 0.0f, CubicInterpolationAlpha);

			const float SmoothHeight2 = FMath::CubicInterp(static_cast<float>(BiomesHeightMap[(J - 1) + I * SizeY]), 0.0f, static_cast<float>(BiomesHeightMap[i]), 0.0f, CubicInterpolationAlpha);

			const float SmoothHeight3 = FMath::CubicInterp(static_cast<float>(BiomesHeightMap[(I + 1) * SizeX + J]), 0.0f, static_cast<float>(BiomesHeightMap[i]), 0.0f, CubicInterpolationAlpha);

			const float SmoothHeight4 = FMath::CubicInterp(static_cast<float>(BiomesHeightMap[(I + 1) * SizeX + J]), 0.0f, static_cast<float>(BiomesHeightMap[i]), 0.0f, CubicInterpolationAlpha);

			const float SmoothHeight5 = FMath::CubicInterp((SmoothHeight1 + SmoothHeight2 + SmoothHeight3 + SmoothHeight4) / 4.0f, 0.0f, static_cast<float>(BiomesHeightMap[i]), 0.0f, 0.5);

			BiomesHeightMap[i] = (SmoothHeight1 + SmoothHeight2 + SmoothHeight3 + SmoothHeight4) / 4.0f;
		}
	}
}

float UFCHeightMapSmoothingFilter::GetSmoothingHeight(const TArray<uint16>& BiomesHeightMap, int32 X, int32 Y, int32 i)
{
	return 0.0f;
}
