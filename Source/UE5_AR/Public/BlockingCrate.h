// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockingCrate.generated.h"

/** A class that has functionality of a mesh that is destroyed  when all enemies of a class die. */

UCLASS()
class UE5_AR_API ABlockingCrate : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Initializes the mesh. */

	ABlockingCrate();


protected:
	/** When the crate is spawned, starts a looping timer which does a check for if all the enemies have been killed.  */

	virtual void BeginPlay() override;


public:	
	/** Tick is disabled. */

	virtual void Tick(float DeltaTime) override;


private:
	/** Function which the looping timer calls. Checks if all the enemies have been killed. Destroys the crate if so. */

	void Action();


	/** The mesh component which will be set as a crate in the blueprint instance. */

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* mesh;


	/** The class of the enemy to be checked. Must be set in the blueprint. */

	UPROPERTY(EditAnywhere, Category = "References")
	TSubclassOf<AActor> enemyClass;


	/** Stores reference to the sound effect that is played when the create is destroyed. */

	UPROPERTY(EditAnywhere, Category = "References")
	USoundBase* crateBreakSound;


	/** The timer handled for the looping check. */

	FTimerHandle conditionCheckTH;


	/** The time in between checks. */

	float checkTime = 1.f;
};
