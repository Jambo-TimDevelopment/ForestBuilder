

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Core/FCControlPoint.h"
#include "FCAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API AFCAIController : public AAIController
{
	GENERATED_BODY()

public:
	/*Default Constructor*/
	AFCAIController();

	virtual void Tick(float DeltaSeconds) override;

	TArray<FPointNavigationGrid> GlobalWay;
	TArray<float> PerspectiveTimeBetweenPoints;
	TArray<float> ResultTimeBetweenPoints;

	float PerspectiveResultTime;
	float ResultResultTime;

	/*Called when the AI Controller possesses a Pawn*/
	virtual void OnPossess(APawn* InPawn) override;

	/*Sets the new sensed target value inside our Blackboard values*/
	void SetSensedTarget(APawn* NewTarget);

	UBlackboardComponent* GetBlackboardComp();

protected:
	/*A Behavior tree component in order to be able to call specific functions like starting our BT*/
	UBehaviorTreeComponent* BehaviorTreeComponent;

	/*A Blackboard component which will be used to initialize our Blackboard Values*/
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationToGoKey;
};
