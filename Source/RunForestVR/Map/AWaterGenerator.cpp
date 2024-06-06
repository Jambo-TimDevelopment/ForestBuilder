#include "AWaterGenerator.h"

#include "FBiome.h"
#include "FWater.h"

TArray<FWater> AWaterGenerator::CreateRiver(TArray<FBiome> Biomes, int X, int Y, int Count)
{
	// fill mass rivers for generation. 1-water be, 0 - no
	for (int I = 0; I < Biomes.Num(); I ++)
	{
		// lakes
		if (Biomes[I].Coordinate.Z <= 0  && Biomes[I].Coordinate.Z >= -400)
		{
			Water.Add(FWater(FVector(Biomes[I].Coordinate.X, Biomes[I].Coordinate.Y, Biomes[I].Coordinate.Z ), 1));
		}
		else Water.Add(FWater(FVector(Biomes[I].Coordinate.X, Biomes[I].Coordinate.Y, Biomes[I].Coordinate.Z), 0));
	}
	//create river
	//step one : get coordinate two points - start and end river
	//step two : get midle perpendiculate for points

	for (int I = 2; I < X - 2; I ++)
	{
		for (int J = 2; J < Y - 2; J ++)
		{
			// check neighbords
			int CountNeighbors = 0;
			// up
			if (Biomes[I * Y + J].TypeBiome == Biomes[(I - 1) * Y + J].TypeBiome)
			{
				CountNeighbors ++;
			}
			//down
			if (Biomes[I * Y + J].TypeBiome == Biomes[(I + 1) * Y + J].TypeBiome)
			{
				CountNeighbors ++;
			}
			//rigth
			if (Biomes[I * Y + J].TypeBiome == Biomes[I * Y + J + 1].TypeBiome)
			{
				CountNeighbors ++;
			}
			//left
			if (Biomes[I * Y + J].TypeBiome == Biomes[I * Y + J - 1].TypeBiome)
			{
				CountNeighbors ++;
			}
			if(CountNeighbors > 1)
			{
				Water[I * Y + J].IsWater = 1;
				Water[I * Y + J + 1].IsWater = 1;
				Water[I * Y + J - 1].IsWater = 1;
			}
		}
	}
	return Water;
}

FVector AWaterGenerator::GetEndRiver()
{
	int IndexEndRiverFind = FMath::RandRange(Water.Num() / 2 - Water.Num() / 4, Water.Num() / 2 + Water.Num() / 4);
	while (Water[IndexEndRiverFind].IsWater != 1)
	{
		IndexEndRiverFind = FMath::RandRange(Water.Num() / 2 - Water.Num() / 4, Water.Num() / 2 + Water.Num() / 4);
	}
	return Water[IndexEndRiverFind].Coordinate;
}

FVector AWaterGenerator::GetStartRiver()
{
	int IndexStartRiverFind = FMath::RandRange(Water.Num() / 2 - Water.Num() / 4, Water.Num() / 2 + Water.Num() / 4);
	while (Water[IndexStartRiverFind].IsWater != 0)
	{
		IndexStartRiverFind = FMath::RandRange(Water.Num() / 2 - Water.Num() / 4, Water.Num() / 2 + Water.Num() / 4);
	}
	return Water[IndexStartRiverFind].Coordinate;
}

FVector AWaterGenerator::GetEuationMiddlePerpendicular(FVector Start, FVector End)
{
	int XM = (Start.X + End.X) / 2;
	int YM = (Start.Y + End.Y) / 2;

	float KAB = (End.Y - Start.Y) / (End.X - End.X);
	float KM = -1 / KAB;

	float B = YM - KM * XM;
	float Ym = KM * XM + (YM - KM * XM);
	FVector Point;
	Point.Y = Ym;
	Point.X = XM;
	return Point;
}
