

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Camera/CameraComponent.h"
#include "Core/ForestCreator.h"
#include "Character/FCCharacter.h"
#include "FCAITouristCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FORESTCREATORPLUGIN_API AFCAITouristCharacter : public AFCCharacter
{
	GENERATED_BODY()
public:
	AFCAITouristCharacter();
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComp;

	/*A Behavior Tree reference*/
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	TWeakObjectPtr<AForestCreator> CachedForestCreator;
};
