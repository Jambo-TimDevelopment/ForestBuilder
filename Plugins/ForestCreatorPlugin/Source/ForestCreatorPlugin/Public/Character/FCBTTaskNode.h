

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Core/FCControlPoint.h"
#include "FCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FCBTTaskNode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FORESTCREATORPLUGIN_API UFCBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
