// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPawn.h"
#include "EnemyPawn.generated.h"

/**
 * The enemy child of the character pawn which adds implementation for chase,
 * running away from, and attacking the player.
 */
UCLASS()
class UE5_AR_API AEnemyPawn : public ACharacterPawn
{
	GENERATED_BODY()

public:
	/** Initialises enemy. */

	AEnemyPawn();

	/** Tick called every frame, calls parents(characters) tick function. */

	virtual void Tick(float DeltaTime) override;


	/** Checks for if the player is on the same planet and is range for chasing.
	 * If the player has star power up, the enemy is set to flee from the player.
	 * @return bool indicating if the enemy should chase the player.
	 */

	bool perceptionCheck();


	/**
	* Gets if the enemy is attacking the player.
	* @return true if the enemy just dealt damage to the player.
	*/

	UFUNCTION(BlueprintPure, Category = "Getters")
	bool GetAttacking() const;


	/**
	* Sets the attacking state of the enemy.
	* Currently only used by the animation blueprint to set,
	* attacking to false after attack animation plays
	* @return true if the enemy just dealt damage to the player.
	*/

	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetAttacking(bool b);

protected:
	/** Called when the enemy is spawned, also gets a reference to the player and stores it save acquiring it multiple times. */

	virtual void BeginPlay() override;

private:
	/** Adds extra functionality to the parents overlap begin,
	 * if the enemy overlaps with player check if they have the star power up, if so die.
	 * else attack the player and put the attack on cool down (looping).
	 */

	virtual void OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


	/** Adds extra functionality to the parents overlap end,
	 * If stopped overlapping with the player cancel the attack timer.
	 */

	virtual void OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	/** Gets a reference to the player and stores it. */

	void GetPlayerRef();


	/** After attack timer has ended check if the player is not dead,
	 * if so deal damage to the player.
	 * otherwise remove the attack timer loop.
	 */

	void AttackPlayer();


	/**
	 * The class of the player pawn used to get the reference.
	 * @warning MUST BE SET IN BLUEPRINT CHILD!
	 */

	UPROPERTY(Category = "References", EditAnywhere)
	TSubclassOf<class APlayerPawn> playerClass;


	/** The reference used to store the player */

	class APlayerPawn* playerRef;


	/** Timer used for looping an attack */

	FTimerHandle attackTH;


	/** The time between attacks by the enemy (time handed into attack timer). */

	float attackRate = 1.5f;


	/** Stores whether the attack animation should be played. */

	bool bAttacking = false;


	/** Stores if the enemy should flee from the player (i.e. if the enemy has star powerup). */

	bool bFleeing = false;
};
