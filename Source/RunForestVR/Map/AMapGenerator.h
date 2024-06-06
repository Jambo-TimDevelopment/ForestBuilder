

#pragma once

#include "CoreMinimal.h"
#include "HeightMap.h"
#include "HeightMap.h"
#include "GameFramework/Actor.h"
#include "AMapGenerator.generated.h"

UCLASS()
class RUNFORESTVR_API AAMapGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
