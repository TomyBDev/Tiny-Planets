// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

/** A camera pawn which has functionality for being controlled by keyboard+mouse or phone. */

UCLASS()
class UE5_AR_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Initializes the Camera Component and arrow component.
	 * If in AR Mode Get the UAR Session Config
	 * If in PC mode initialize floating pawn movement component.
	 */

	ACameraPawn();

protected:
	/** Called when camera is spawned, Sets variables, and gets references. */

	virtual void BeginPlay() override;


public:	
	/** Called every frame. */

	virtual void Tick(float DeltaTime) override;


	/** Binds the inputs actions and axis, binds different inputs based on if in PC or AR mode. */

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	/** The camera component controlled by the user. */

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* camera;


	/** An arrow component which is used to indicate the direction of the player  */

	UPROPERTY(EditAnywhere, Category = "Components")
	class UArrowComponent* arrow;


	/** The floating pawn movement component use in pc mode for the camera movement. */

	UPROPERTY(EditAnywhere, Category = "Components")
	class UFloatingPawnMovement* floatingPawnMovement;


	/** A reference to the class of the player. Must be set in blueprint instance! */

	UPROPERTY(Category = "References", EditAnywhere)
	TSubclassOf<class APlayerPawn> playerClass;


	/** Toggles whether the camera is in AR mode or PC mode. Can be set in the blueprint instance. */

	UPROPERTY(Category = "CameraMode", EditAnywhere)
	bool bARMode;


	/** The speed of the PC pawns movement. Can be set in the blueprint instance. */

	UPROPERTY(Category = "Setting", EditAnywhere)
	float floatSpeed = 100.f;


	/** Bind Axis move functions PC. */
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);


	/**
	 * On mouse clicked, raycast from center of the screen,
	 * if the raycast hit an actor, register the tap with the character.
	 *
	 */
	void OnClickPressed();


	/** Toggles showing the mouse cursor on escape being pressed. */

	void OnEscapePressed();


	/**
	 * On screen pressed, raycast from tap position,
	 * if the raycast hit an actor, register the tap with the character.
	 *
	 * @param FingerIndex Enum containing how many fingers being pressed.
	 * @param ScreenPos The position on screen being tapped.
	 */
	virtual void OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);


	/** Gets and store reference to the player. */

	void GetPlayerRef();


	/** A pointer to store a reference to the player. */

	APlayerPawn* playerRef;


	/** A pointer to store a reference to the game mode. */

	class ACustomGameMode* gameModeRef;


	/** A pointer to store a reference to the player controller. */

	class ACustomPlayerController* playerControllerRef;


	/** Boolean that tracks the state of if the cursor is being shown or not. */

	bool bShowCursor = false;

};
