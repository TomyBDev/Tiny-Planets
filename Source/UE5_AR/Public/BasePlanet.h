// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlanetTypeEnum.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlanet.generated.h"

/** Planet base class, which contains base functionality for different shaped planets. */

UCLASS()
class UE5_AR_API ABasePlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Initializes the mesh component for the planet. */

	ABasePlanet();

protected:
	/**  Called when planet is spawned. */

	virtual void BeginPlay() override;


public:	
	/** Called every frame. */

	virtual void Tick(float DeltaTime) override;


	/** Returns an enum with the type of planet the actor is. This is value is set in the child implementations. */

	EPlanetTypeEnum GetPlanetType() const;


protected:
	/** The planets mesh component */

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* mesh;


	/** The priority level to set the planets gravity field to. Highest priority number takes effect. */

	UPROPERTY(EditAnywhere, Category = "Settings")
	int priorityLevel = 1;


	/** A reference to store the gravity field which will be attached to the planet. */

	class AGravityField* gravityField;


	/** A enumerator to store the planet type, will be overwritten in child implementation. */

	EPlanetTypeEnum planetType = EPlanetTypeEnum::FlatGround;
};
