


#include "Character/FCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/FCControlPoint.h"
#include "Character/FCAITouristCharacter.h"
#include "Kismet/GameplayStatics.h"


AFCAIController::AFCAIController()
{
	//Initialize our components
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	LocationToGoKey = "LocationToGo";
}

void AFCAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void AFCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//If our character is valid and has a valid Behavior Tree,
	//Initialize the values of the Blackboard and start the tree
	AFCAITouristCharacter* AITouristCharacter = Cast<AFCAITouristCharacter>(InPawn);
	if (AITouristCharacter && AITouristCharacter->BehaviorTree->BlackboardAsset)
	{
		//Initialize the blackboard values
		BlackboardComponent->InitializeBlackboard(*AITouristCharacter->BehaviorTree->BlackboardAsset);

		//Start the tree
		BehaviorTreeComponent->StartTree(*AITouristCharacter->BehaviorTree);
	}

	//Start the behavior tree which corresponds to the specific character
	BehaviorTreeComponent->StartTree(*AITouristCharacter->BehaviorTree);
}

void AFCAIController::SetSensedTarget(APawn* NewTarget)
{
	//Set a new target to follow
	if (BlackboardComponent) BlackboardComponent->SetValueAsObject(LocationToGoKey, NewTarget);
}

UBlackboardComponent* AFCAIController::GetBlackboardComp()
{
	return BlackboardComponent;
}