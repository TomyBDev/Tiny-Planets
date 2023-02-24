// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUpTypeEnum.h"

#include "CoreMinimal.h"
#include "CharacterPawn.h"
#include "PlayerPawn.generated.h"

/**
 *The player child of the character pawn which adds implementation for interacting with collectables,
 *and responding to screen taps from the user.
 */
UCLASS()
class UE5_AR_API APlayerPawn : public ACharacterPawn
{
	GENERATED_BODY()

public:
	/** Initialises player. */

	APlayerPawn();


	/** Tick called every frame, calls parents(characters) tick function. */

	virtual void Tick(float DeltaTime) override;


	/**
	 * When the screen is tapped, the location of the tap and the actor that was tapped are passed in.
	 * First checking if the ground tapped is the ground the character is on.
	 * If so let the character know the point to move (also raise it off the ground) to and begin movement.
	 * @param impactPoint - The location of the tap in world space.
	 * @param actor - The actor that was tapped.
	 */

	void NewTap(FVector impactPoint, AActor* actor);


	/**
	 * Gets a reference to the planet the player is currently standing on.
	 * @return Pointer of ABasePlanet which all planets derive from.
	 */

	ABasePlanet* GetGroundReference() const;


	/**
	 * Gets a reference to the current power up the player has.
	 * @return Enum which stores the current power up state of the player.
	 */

	EPowerUpTypeEnum GetPowerUpTypeEnum() const;

	/**
	 * Based on the new power up the player will either:
	 * Increment the counter of the collectable (if its a type of coin),
	 * Or apply the power up otherwise.
	 * @param powerUpType - Enum of the power up the player should be set to.
	 * @param duration - The duration the power up should last.
	 */

	void SetPowerUpTypeEnum(EPowerUpTypeEnum powerUpType, float duration);


protected:
	/** When the player is spawned store the starting position for respawning, and store a reference of the custom gamemode. */

	virtual void BeginPlay() override;


	/**
	 * Overrides the parents implementation of kill, but first calls the parents implementation which sets the character in death mode.
	 * Then lets the game mode know the player is dead, then proceeds with respawning the player if they have lives remaining.
	 * A respawn timer is set which will then complete the respawn process after a set amount of time.
	 */
	virtual void Kill() override;

private:
	/**
	 * If the power up was a star the material is set back to default.
	 * Otherwise if the power up was a speed power up the speed lines,
	 * are removed and speed returned to walk speed.
	 */

	void EndPowerUp();


	/**
	* Once the respawn timer completes this function is called, this function resets all the player variables back to their default.
	* (Such as health, speed, tick, visibility, location, and collision).
	*/

	void EndRespawn();


	/** A reference to the sound used when a coin is picked up */

	UPROPERTY(EditAnywhere, Category = "References")
	USoundBase* coinSoundBase;


	/** A reference to the sound used when a power up is picked up */

	UPROPERTY(EditAnywhere, Category = "References")
	USoundBase* powerupSoundBase;


	/** A reference to the normal material on the player to revert to after being in star power up mode */

	UPROPERTY(EditAnywhere, Category = "References")
	UMaterialInstance* normalMat;


	/** A reference to a yellow material used to represent star mode on the player */

	UPROPERTY(EditAnywhere, Category = "References")
	UMaterialInstance* starMat;


	/** The speed to to change to when the speed power up is used. */

	UPROPERTY(EditAnywhere, Category = "Settings")
	float sprintSpeed = 80.f;


	/** The time for the power to disappear for during respawn */

	UPROPERTY(EditAnywhere, Category = "Settings")
	float respawnTime = 5.f;


	/** The timer handles used for the power up and respawn timer */

	FTimerHandle powerUpTH;
	FTimerHandle respawnTH;


	/** Enum to store the current power up state of the player */

	EPowerUpTypeEnum powerUp = EPowerUpTypeEnum::None;


	/** Used to store the reference of the game state, to save acquiring it multiple times. */

	class ACustomGameStateBase* gamestateRef;


	/** A vector used to store the location at which the player should be spawned at. */

	FVector spawnLocation = { 0,0,0 };
};
