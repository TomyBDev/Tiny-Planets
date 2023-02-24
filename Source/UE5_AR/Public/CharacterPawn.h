// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlanetTypeEnum.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

/** Character base class, for base function of player and enemies. */

UCLASS()
class UE5_AR_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Initializes the mesh and collision sphere.*/

	ACharacterPawn();


	/** Called every frame, used to update the movement of the player. */

	virtual void Tick(float DeltaTime) override;


	/** The players death status */

	UFUNCTION(BlueprintPure, Category = "Getters")
	bool GetIsDead() const;


	/** The players falling status (Is player not touching ground). */

	UFUNCTION(BlueprintPure, Category = "Getters")
	bool GetIsFalling() const;


	/** The speed of the player, based on its movement in the direction parallel to the ground. */

	UFUNCTION(BlueprintPure, Category = "Getters")
	float GetSpeed() const;


	/** Returns the direction of gravity, used by animation blueprint for footstep checks. */

	UFUNCTION(BlueprintPure, Category = "Getters")
	FVector GetGravityDirection() const;


	/** When the player takes damage, play haptics and check if the player has died. */

	virtual float TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	/** Puts the character into the killed state. */

	virtual void Kill();


protected:
	/** When character collides with gravity field, store a reference to it. */

	UFUNCTION()
	virtual void OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/** When player leaves gravity field, if is the gravity field currently in effect. Then checks for the highest priority gravity field and stores it reference if there is one. */

	UFUNCTION()
	virtual	void OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/** Called when character is spawned. initialize things such as health, speed and gravity direction. */

	virtual void BeginPlay() override;


	/** The characters skeletal mesh. */

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* mesh;


	/** A collision sphere used for generating overlaps with other actors and pawns. */

	UPROPERTY(EditAnywhere, Category = "Components")
		class USphereComponent* sphereComponent;


	/** The characters maximum health, can be changed in blueprint children. */

	UPROPERTY(EditAnywhere, Category = "Settings")
		float maxHealth = 100.f;


	/** The characters maximum speed, can be changed in blueprint children. */

	UPROPERTY(EditAnywhere, Category = "Settings")
		float walkSpeed = 60.f;


	/** Position vector that the player will move towards. */

	FVector targetPos = { 0,0,0 };


	/** A directional vector point in the direction of gravity (Changes during gameplay). */

	FVector gravityDir = { 0,0,0 };


	/** Used to store the radius of the collision sphere component. */

	float sphereRadius = 0;


	/** Used to tell the player if they should move or not. */

	bool bMoving = false;


	/** Tracks the state of the players death */

	bool bDead = false;


	/** A reference to the planet or ground the player is standing on. */

	class ABasePlanet* groundRef;


	/** Enum which store what type of movement should be used, is based on the planet type. */

	EPlanetTypeEnum movementType;


	/** A pointer to store reference to the custom gamemode. */

	class ACustomGameMode* gameModeRef;


	/** A pointer to store reference to the custom gamemode. */

	class UCustomGameInstance* gameInstanceRef;


	/** The current health of the player (initially set to max health). */

	float health = 100.f;


	/** The current speed of the player (initially set to max speed, the speed value is only changed during runtime in the player child class). */

	float speed = 60.f;

private:

	/** Handles movement and rotation of the character. */

	void MoveCharacter(float dt);


	/** Calculates the gravity velocity, and determines if the player should rotate to the orientation of the new gravity direction. Also performs the check for if the player is on the ground. */

	void Gravity();


	/** Check if the player is standing on ground, and update the boolean tracking if the player is on the ground. */

	void GroundCheck();


	/** Gets all the overlapping gravity field and store a reference to the one with the highest priority. */

	void GetNewGravField();


	/** Check if there is space for the character to move forward. Setting the movement boolean based if there is space. */

	void ForwardCheck(float dt);


	/** The scalar component of gravity. (How fast character accelerates) */

	UPROPERTY(EditAnywhere, Category = "Settings")
	float gravityStrength = 30.f;


	//** Height from which the character should flip to match gravity (the rotation of the character) */

	UPROPERTY(EditAnywhere, Category = "Settings")
	float flipHeight = 25.f;


	/** a pointer to the current field. */

	class AGravityField* currentfield;


	/** Vector to store the velocity of the character parallel to the ground. */

	FVector velocity = {0,0,0};


	/** Vector to store the gravity velocity. */

	FVector gravity = {0,0,0};


	/** Vector to store the direction of the character parallel to the ground. */

	FVector movementDir = { 0,0,0 };


	/** Store whether the player is on the ground. */

	bool bGrounded = true;


	/** Determines whether the player should rotate (due to gravity change of direction). Only rotates to correct orientation when closer to the ground. */

	bool bShouldRotate = false;

};
