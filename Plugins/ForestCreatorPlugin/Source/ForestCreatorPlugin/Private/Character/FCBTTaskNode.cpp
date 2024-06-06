


#include "Character/FCBTTaskNode.h"
#include "Core/FCControlPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/FCAIController.h"
#include "Core/FCNavigationGridBuilder.h"

EBTNodeResult::Type UFCBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AFCAIController* FCAIController = Cast<AFCAIController>(OwnerComp.GetAIOwner());

	/*If the Controller is valid:
	1)Get the Blackboard Component and the Current Point of the bot
	2)Search for the next point, which will be different from the Current Point*/
	if (FCAIController)
	{
		UBlackboardComponent* BlackboardComponent = FCAIController->GetBlackboardComp();
		FVector CurrentControlPointLocation = BlackboardComponent->GetValueAsVector("LocationToGo");

		if (FCAIController->GlobalWay.IsEmpty())
		{
			return EBTNodeResult::Failed;
		}

		FPointNavigationGrid TargetPoint = FCAIController->GlobalWay.Last();

		const FVector CurrentActorLocation = FCAIController->GetPawn()->GetActorLocation();
		FVector NextTargetPointLocation = TargetPoint.Location;
		if ((CurrentActorLocation - NextTargetPointLocation).Length() <= 250)
		{
			FCAIController->GlobalWay.Pop(true);
			if (FCAIController->GlobalWay.IsEmpty())
			{
				return EBTNodeResult::Failed;
			}
			TargetPoint = FCAIController->GlobalWay.Last();
			NextTargetPointLocation = TargetPoint.Location;
		}

		//Find a next point which is different from the current one
		/*do
		{
			RandomIndex = FMath::RandRange(0, ForestCreator.Num() - 1);
			//Remember that the Array provided by the Controller function contains AActor* objects so we need to cast.
			NextTargetPoint = Cast<AFCControlPoint>(ForestCreator[RandomIndex]);
			NextTargetPointLocation = NextTargetPoint->GetActorLocation();
		}
		while (CurrentControlPointLocation == NextTargetPointLocation);
*/
		//Update the next location in the Blackboard so the bot can move to the next Blackboard value
		BlackboardComponent->SetValueAsVector("LocationToGo", NextTargetPointLocation);

		//At this point, the task has been successfully completed
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
