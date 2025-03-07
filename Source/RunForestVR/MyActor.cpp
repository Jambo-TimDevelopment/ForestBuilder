


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	SetRandomRotationSpeed();
}

void AMyActor::SetRandomRotationSpeed()
{
	CurrentRotationSpeed = FMath::RandRange(MinimumRotationSpeed, MaximumRotationSpeed);
	GEngine->AddOnScreenDebugMessage(0, 5, TextColor,
	                                 FString::Printf(TEXT("Inital rotation speed %f"), CurrentRotationSpeed));
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotator = GetActorRotation();
	Rotator.Yaw += CurrentRotationSpeed * DeltaTime;
	SetActorRotation(Rotator);
}
