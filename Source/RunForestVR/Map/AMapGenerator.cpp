


#include "AMapGenerator.h"


// Sets default values
AAMapGenerator::AAMapGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAMapGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

