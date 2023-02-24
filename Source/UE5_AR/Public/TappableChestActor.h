// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TappableActor.h"
#include "TappableChestActor.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API ATappableChestActor : public ATappableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATappableChestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Action() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category="References", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACollectable> collectableToSpawnClass;
	ACollectable* collectableRef;
};
