// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

#include "Camera/CameraComponent.h"
#include "ARBlueprintLibrary.h"
#include "CustomGameMode.h"
#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TappableActor.h"
#include "Components/ArrowComponent.h"


ACameraPawn::ACameraPawn()
{
	/// Enables the tick function to run.
	PrimaryActorTick.bCanEverTick = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	SetRootComponent(camera);

	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	arrow->SetupAttachment(camera);

	if (bARMode)
	{
		// Get UAR session config, if in AR mode.
		static ConstructorHelpers::FObjectFinder<UARSessionConfig>
			ConfigAsset(TEXT("ARSessionConfig'/Game/Blueprints/HelloARSessionConfig.HelloARSessionConfig'"));
	}
	else //Create floating pawn movement component, if in PC mode.
		floatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
}


void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	floatingPawnMovement->MaxSpeed = floatSpeed;

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	gameModeRef = Cast<ACustomGameMode>(GM);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	playerControllerRef = Cast<ACustomPlayerController>(PC);

	if (!bARMode)
	{
		camera->bUsePawnControlRotation = true;
		camera->FieldOfView = 90;
	}
}


void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(playerRef) || !IsValid(playerControllerRef))
	{
		//Player is spawned after camera pawn, so might need to find ref after begin play.
		GetPlayerRef();
		return;
	}

	//Check if player is not on screen
	FVector playerLoc = playerRef->GetActorLocation();
	FVector2D screenLoc;
	bool bSucess = playerControllerRef->ProjectWorldLocationToScreen(playerLoc, screenLoc);

	// If the player is not on screen and is not dead show an arrow pointing at player.
	if (!bSucess && !playerRef->GetIsDead())
	{
		arrow->SetWorldRotation((playerRef->GetActorLocation() - GetActorLocation()).Rotation());
		arrow->SetHiddenInGame(false);
	}
	else if (!arrow->bHiddenInGame) // If the player is on screen hide the arrow.
		arrow->SetHiddenInGame(true);
}


void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup the player input binds, based on if using AR or PC.
	if (bARMode)
	{
		PlayerInputComponent->BindTouch(IE_Pressed, this, &ACameraPawn::OnScreenTouch);
	}
	else
	{
		PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ACameraPawn::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("Right"), this, &ACameraPawn::MoveRight);
		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
		PlayerInputComponent->BindAxis(TEXT("Up"), this, &ACameraPawn::MoveUp);

		PlayerInputComponent->BindAction(TEXT("Click"), IE_Pressed, this, &ACameraPawn::OnClickPressed);

		PlayerInputComponent->BindAction(TEXT("ShowCursor"), IE_Pressed, this, &ACameraPawn::OnEscapePressed);
	}

}


void ACameraPawn::GetPlayerRef()
{
	AActor* actor = UGameplayStatics::GetActorOfClass(this, playerClass);
	APlayerPawn* PR = Cast<APlayerPawn>(actor);
	if (IsValid(PR))
	{
		playerRef = PR;
	}
}


void ACameraPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	if (!IsValid(gameModeRef))
		return;

	if (!IsValid(playerRef))
		GetPlayerRef();

	FHitResult hitResult;

	// Run a line trace from where the screen was tapped.
	if (gameModeRef->WorldHitTest(hitResult, ScreenPos))
	{
		// If the line trace succeeded in hitting an actor and the game has started.
		if (IsValid(hitResult.GetActor()) && gameModeRef->GetGameStarted())
		{
			if (IsValid(playerRef))
			{
				// Inform the player of the new tap location and the actor that was tapped.
				playerRef->NewTap(hitResult.ImpactPoint, hitResult.GetActor());
			}

			ATappableActor* tappableActorRef = Cast<ATappableActor>(hitResult.GetActor());
			if (IsValid(tappableActorRef))
			{
				// If the tapped actor is the tappable actor, inform that actor to do the action.
				tappableActorRef->Action();
			}
		}
	}
}


void ACameraPawn::MoveForward(float AxisValue)
{
	AddMovementInput(camera->GetForwardVector(), AxisValue);
}


void ACameraPawn::MoveRight(float AxisValue)
{
	AddMovementInput(camera->GetRightVector(), AxisValue);
}


void ACameraPawn::MoveUp(float AxisValue)
{
	AddMovementInput(camera->GetUpVector(), AxisValue);
}


void ACameraPawn::OnClickPressed()
{
	if (!IsValid(playerRef))
		GetPlayerRef();

	FHitResult hitResult;
	FVector startPos = camera->GetComponentLocation();
	FVector endPos = startPos + (camera->GetForwardVector() * 10000.f);

	// Run a line trace from the camera is looking.
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, ECC_WorldDynamic))
	{
		// If the line trace succeeded in hitting an actor and the game has started.
		if (IsValid(hitResult.GetActor()))
		{
			if (IsValid(playerRef))
			{
				// Inform the player of the new tap location and the actor that was tapped.
				playerRef->NewTap(hitResult.ImpactPoint, hitResult.GetActor());
			}

			ATappableActor* tappableActorRef = Cast<ATappableActor>(hitResult.GetActor());
			if (IsValid(tappableActorRef))
			{
				// If the tapped actor is the tappable actor, inform that actor to do the action.
				tappableActorRef->Action();
			}
		}
	}
}


void ACameraPawn::OnEscapePressed()
{
	//Check if the game has started (since player only exists if that is true).
	if (!IsValid(playerRef))
		return;

	//Toggle cursor showing
	playerControllerRef->SetShowMouseCursor(bShowCursor);
	bShowCursor = !bShowCursor;
}