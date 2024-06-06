


#include "Character/FCCharacter.h"
#include "ForestCreatorUtils.h"
#include "Landscape.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Core/FCBiomesDataAsset.h"
#include "Core/FCControlPoint.h"
#include "Core/ForestCreator.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

class AForestCreator;
// Sets default values
AFCCharacter::AFCCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Disable some environmental shadowing to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFCCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
}

// Called every frame
void AFCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = GetSpeedByBiomes();
}

void AFCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("Move Forward/Outforward", this, &AFCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right/Left", this, &AFCCharacter::MoveRight);
	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFCCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AFCCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AFCCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFCCharacter::AddControllerPitchInput);
	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFCCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFCCharacter::StopJump);
}

void AFCCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFCCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFCCharacter::StartJump()
{
	bPressedJump = true;
}

void AFCCharacter::StopJump()
{
	bPressedJump = false;
}

float AFCCharacter::GetSpeedByBiomes()
{
	UWorld* World = nullptr;
	{
		// We want to create the landscape in the landscape editor mode's world
		FWorldContext& EditorWorldContext = GEditor->GetEditorWorldContext();
		World = EditorWorldContext.World();
	}

	if (!CachedForestCreator.IsValid())
	{
		CachedForestCreator = StaticCast<AForestCreator*>(UGameplayStatics::GetActorOfClass(GetWorld(), AForestCreator::StaticClass()));
		if (!flag && CachedForestCreator->ControlPoints.Contains(0) && IsValid(CachedForestCreator->ControlPoints[0]))
		{
			flag = true;
			SetActorLocation(CachedForestCreator->ControlPoints[0]->GetActorLocation());
		}
		if (!CachedForestCreator.IsValid())
		{
			return 0.0f;
		}
	}

	FVector MyCharacter = AFCCharacter::GetActorLocation();
	FHitResult Hit;
	const FCollisionQueryParams CollisionQueryParams;
	const FCollisionResponseParams CollisionResponseParams;
	FVector Start(MyCharacter.X, MyCharacter.Y, 1000000);
	FVector End(MyCharacter.X, MyCharacter.Y, -1000000);
	int32 Z = 0;
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldDynamic, CollisionQueryParams, CollisionResponseParams))
	{
		Z = Hit.Location.Z;
	}
	FLandscapeParameters LandscapeParameters = CachedForestCreator->LandscapeParameters;
	const int32 QuadsPerComponent = LandscapeParameters.SectionSize * LandscapeParameters.SectionsPerComponent;
	const int32 SizeX = LandscapeParameters.ComponentCountX * QuadsPerComponent + 1;
	const int32 SizeY = LandscapeParameters.ComponentCountY * QuadsPerComponent + 1;

	int32 I = Hit.Location.X / LandscapeParameters.SectionScale.X;
	int32 J = Hit.Location.Y / LandscapeParameters.SectionScale.Y;
	int32 i = J * SizeY + I;
	if (LandscapeParameters.BiomesMask.Num() <= i || i < 0)
	{
		return 0;
	}
	int32 BiomesIndex = CachedForestCreator->LandscapeParameters.BiomesMask[i];
	TArray<UFCBiomesDataAsset*> Biomes = CachedForestCreator->GetBiomesSet().Array();
	float BiomesSpeed = Biomes[BiomesIndex]->BiomesSpeed;
	return BiomesSpeed;
}
