

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class RUNFORESTVR_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetRandomRotationSpeed();
	
	UPROPERTY(EditAnywhere)
	int MyFirstProperty = 100;
	
	UPROPERTY(EditAnywhere)
	FColor TextColor = FColor::Red;

	UPROPERTY(EditAnywhere);
	float MinimumRotationSpeed = 30;

	UPROPERTY(EditAnywhere);
	float MaximumRotationSpeed = 360;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	float CurrentRotationSpeed= 0.0f;

};
