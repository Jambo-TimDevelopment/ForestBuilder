#pragma once
#include "FBiome.h"
#include "FWater.h"


class AWaterGenerator
{
public:
	TArray<FWater> Water;
	
	FVector IndexStartRiver;
	
	FVector IndexEndRiver;
	
	TArray<FWater> CreateRiver(TArray<FBiome> Bioms,int X, int Y,  int Count);
	
	FVector GetStartRiver();
	
	FVector GetEndRiver();
	
	FVector GetEuationMiddlePerpendicular(FVector Start, FVector End);
	
	FVector NewPoint;
};
