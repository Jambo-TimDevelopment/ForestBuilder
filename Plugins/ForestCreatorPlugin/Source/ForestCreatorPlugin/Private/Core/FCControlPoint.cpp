


#include "Core/FCControlPoint.h"

#include "CompGeom/FitOrientedBox3.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFCControlPoint::AFCControlPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
	TextRenderComponent->SetupAttachment(VisualMesh);

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
		VisualMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		VisualMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		VisualMesh->SetGenerateOverlapEvents(true);
	}
}

void AFCControlPoint::DrawDebugInformation(int32 Index)
{
	TextRenderComponent->SetText(FText::FromString(FString::SanitizeFloat(Index, 0)));
}
