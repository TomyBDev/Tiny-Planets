// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlanet.h"
#include "FlatPlanet.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AFlatPlanet : public ABasePlanet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlatPlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float height = 300.f;
};
