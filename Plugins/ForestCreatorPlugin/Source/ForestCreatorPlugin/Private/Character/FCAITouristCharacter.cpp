


#include "Character/FCAITouristCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFCAITouristCharacter::AFCAITouristCharacter()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing our Pawn Sensing comp and our behavior tree reference
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTreeReference"));
}

// Called when the game starts or when spawned
void AFCAITouristCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFCAITouristCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = GetSpeedByBiomes();
}

// Called to bind functionality to input
void AFCAITouristCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

