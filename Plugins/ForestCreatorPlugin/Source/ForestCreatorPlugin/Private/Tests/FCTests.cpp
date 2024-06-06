


#include "Tests/FCTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "RunForestVR.Math.MaxInt",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestPerlinMaskValue, "RunForestVR.Math.TestPerlinMaskValue",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority);


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestNeighborsNavigationGrid, "RunForestVR.Math.FTestNeighborsNavigationGrid",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestCalculationBiomesIndex, "RunForestVR.Math.FTestCalculationBiomesIndex",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
                                 EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
	TestTrue("2 different positive numbers", FMath::Max(13, 25) == 25);
	return true;
}


bool FTestPerlinMaskValue::RunTest(const FString& Parameters)
{
	float PerlinNoiseScale = 0.1f;
	int32 SizeX = 100;
	int32 SizeY = 100;
	int32 BiomesMaskNum = SizeX * SizeY;
	TArray<int32> RawHeight;

	int32 BiomesSetNum = 5;

	for (int32 i = 0; i < BiomesMaskNum; i++)
	{
		const uint16 I = i % SizeX;
		const uint16 J = i / SizeY;

		const FVector2d PerlinPosition = FVector2d(I * PerlinNoiseScale, J * PerlinNoiseScale);
		RawHeight.Add((FMath::PerlinNoise2D(PerlinPosition) + 1) / 2 * (BiomesSetNum + 1));
	}

	for (int i = 0; i < BiomesSetNum; i++)
	{
		TestTrue("Biomes contains", RawHeight.Contains(i) == true);
	}
	RawHeight.Empty();
	return true;
}

bool FTestNeighborsNavigationGrid::RunTest(const FString& Parameters)
{
	float PerlinNoiseScale = 0.1f;
	TestTrue("2 different positive numbers", FMath::Max(13, 25) == 25);

	int32 SizeX = 10;
	int32 SizeY = 10;
	int32 SizeGrid = 100;

	struct ExpectedResultNeighbors
	{
		int32 IndexPoint;
		int32 ExpectedResultNeighborsCount;
	};

	TArray<ExpectedResultNeighbors> ResultNeighbors = {
		{0, 3},
		{3, 5},
		{9, 3},
		{90, 3},
		{70, 5},
		{69, 5},
		{95, 5},
		{99, 3}
	};
	for (const auto Data : ResultNeighbors)
	{
		int32 NowCountNeighbors = 0;
		// TODO check Up left point
		if (Data.IndexPoint % SizeX > 0 && Data.IndexPoint / SizeX - 1 >= 0)
		{
			int32 IndexNeighbors = Data.IndexPoint - SizeX - 1;
			NowCountNeighbors++;
		}
		// TODO check Up point
		if (Data.IndexPoint - SizeX >= 0)
		{
			int32 IndexNeighbors = Data.IndexPoint - SizeX;
			NowCountNeighbors++;
		}
		// TODO check Up right point
		if (Data.IndexPoint - SizeX >= 0 && Data.IndexPoint % SizeY != SizeY - 1)
		{
			int32 IndexNeighbors = Data.IndexPoint - SizeX + 1;
			NowCountNeighbors++;
		}
		// TODO check right point
		if (Data.IndexPoint % SizeY != SizeY - 1)
		{
			int32 IndexNeighbors = Data.IndexPoint + 1;
			NowCountNeighbors++;
		}
		// TODO check down right point j - SizeX > 0 && j % SizeY != SizeY - 1
		if (Data.IndexPoint + SizeX < SizeGrid && Data.IndexPoint % SizeY != SizeY - 1)
		{
			int32 IndexNeighbors = Data.IndexPoint + SizeX + 1;
			NowCountNeighbors++;
		}
		// TODO check down point
		if (Data.IndexPoint + SizeX < SizeGrid)
		{
			int32 IndexNeighbors = Data.IndexPoint + SizeX;
			NowCountNeighbors++;
		}
		// TODO check down left point
		if (Data.IndexPoint + SizeX < SizeGrid && Data.IndexPoint % SizeX > 0)
		{
			int32 IndexNeighbors = Data.IndexPoint + SizeX - 1;
			NowCountNeighbors++;
		}
		// TODO check left point
		if (Data.IndexPoint % SizeX > 0)
		{
			int32 IndexNeighbors = Data.IndexPoint - 1;
			NowCountNeighbors++;
		}
		TestTrue("CountNeighbors", NowCountNeighbors == Data.ExpectedResultNeighborsCount);
	}

	return true;
}

bool FTestCalculationBiomesIndex::RunTest(const FString& Parameters)
{
	const int32 SizeX = 505;
	const int32 SizeY = 505;
	const int32 SectionScaleX = 100;
	const int32 SectionScaleY = 100;
	TArray<uint8> BiomesMask;
	BiomesMask.SetNum(SizeX * SizeY);

	for(int32 i =0; i < 10000; i++)
	{
		int32 X = FMath::RandRange(0, SizeX * (int32)SectionScaleX);
		int32 Y = FMath::RandRange(0, SizeY * (int32)SectionScaleY);

		int32 I = X / SectionScaleX;
		int32 J = Y / SectionScaleX;
		int32 Index = J * SizeY + I;
		bool IndexExist = true;
		if (Index > BiomesMask.Num() && Index < 0)
		{
			IndexExist = false;
		}
		TestTrue("Index exist", IndexExist == true);
	}
	return true;
}
