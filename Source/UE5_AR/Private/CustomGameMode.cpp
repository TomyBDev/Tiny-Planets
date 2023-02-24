// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "CameraPawn.h"
#include "CustomGameStateBase.h"
#include "HelloARManager.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "CustomPlayerController.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h" 
#include "CustomGameInstance.h"

ACustomGameMode::ACustomGameMode()
{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	// Set the default pawn and gamestate to be our custom pawn and gamestate programatically
	DefaultPawnClass = ACameraPawn::StaticClass();
	GameStateClass = ACustomGameStateBase::StaticClass();
}

void ACustomGameMode::StartPlay()
{
	// This function will transcend to call BeginPlay on all the actors 
	Super::StartPlay();

	HelloARManager = GetWorld()->SpawnActor<AHelloARManager>();

	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	gameStateRef = Cast<ACustomGameStateBase>(GS);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerControllerRef = Cast<ACustomPlayerController>(PC);
}

void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Making sure the actor remains on the ARPin that has been found.
	if (!IsValid(worldPin))
		return;

	auto TrackingState = worldPin->GetTrackingState();

	switch (TrackingState)
	{
	case EARTrackingState::Tracking:
		if (IsValid(levelRef))
			levelRef->LevelTransform = worldPin->GetLocalToWorldTransform();
		break;

	case EARTrackingState::NotTracking:
		worldPin = nullptr;
		break;

	default:
		// No Action
		break;
	
	}
	
}

bool ACustomGameMode::WorldHitTest(FHitResult& hitResult, FVector ScreenPos)
{
	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(playerController))
		return false;

	FVector WorldPos;
	FVector WorldDir;
	bool bHit = false;

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);

	const FVector endPosition = WorldPos + (WorldDir * 100000.f);

	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	UCustomGameInstance* customGameInstace = Cast<UCustomGameInstance>(GI);

	if (IsValid(customGameInstace) && !customGameInstace->GetMainMenu())
	{
		if (levelSpawnStatus == ESpawnStatus::NOTSPAWNED)
		{
			bHit = TrySpawnGameWorld(WorldDir, ScreenPos);
		}
		else
		{
			bHit = GetWorld()->LineTraceSingleByChannel(hitResult, WorldPos, endPosition, ECC_WorldDynamic);
		}

	}
	return bHit;
}

bool ACustomGameMode::GetReadyToStart() const
{
	if (levelSpawnStatus == ESpawnStatus::NOTSPAWNED)
		return false;
	return true;
}

bool ACustomGameMode::GetGameStarted() const
{
	return bGameStarted;
}

void ACustomGameMode::SetGameStarted()
{
	bGameStarted = true;
	ambienceRef = UGameplayStatics::SpawnSound2D(GetWorld(), ambienceSoundBase);
	musicRef = UGameplayStatics::SpawnSound2D(GetWorld(), musicSoundBase);

	if (IsValid(HelloARManager))
	{
		HelloARManager->StopTrackingPlanes();
	}
}

void ACustomGameMode::SetWorldPlaced()
{
	levelSpawnStatus = ESpawnStatus::SPAWNLOCKED;
}

void ACustomGameMode::PickupWorld()
{
	UGameInstance* GS = UGameplayStatics::GetGameInstance(GetWorld());
	UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(GS);
	if (IsValid(gameInstance))
		gameInstance->SetQuickRestart(true);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("p_GamplayLevel"));
}

bool ACustomGameMode::TrySpawnGameWorld(FVector WorldDir, FVector ScreenPos)
{
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);

	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{

		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			//Spawn the actor pin and get the transform
			UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());

			// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
			if (ActorPin)
			{
				//If the pin is valid 
				auto PinTF = ActorPin->GetLocalToWorldTransform();
				//Spawn a new Actor at the location if not done yet
				FLatentActionInfo LatentInfo;
				UGameplayStatics::LoadStreamLevel(this, TEXT("s_GameMap"), false, true, LatentInfo);
				levelRef = UGameplayStatics::GetStreamingLevel(this, TEXT("s_GameMap"));
				levelRef->LevelTransform = PinTF;
				levelRef->SetShouldBeVisible(true);
				worldPin = ActorPin;
				levelSpawnStatus = ESpawnStatus::SPAWNED;
				return true;
			}
		}
	}

	return false;
}


void ACustomGameMode::PlayerKilled()
{
	if (!IsValid(gameStateRef))
		return;

	gameStateRef->AddLives(-1);

	if (gameStateRef->GetLives() > 0)
		return;

	if (IsValid(ambienceRef))
		ambienceRef->SetActive(false);
	if (IsValid(musicRef))
		musicRef->SetActive(false);

	if (IsValid(playerControllerRef))
		playerControllerRef->GameHasEnded(playerControllerRef->GetPawn(), false);
}

void ACustomGameMode::SetGameWon()
{
	if (IsValid(ambienceRef))
		ambienceRef->SetActive(false);
	if (IsValid(musicRef))
		musicRef->SetActive(false);

	if (!IsValid(playerControllerRef))
		return;

	playerControllerRef->GameHasEnded(playerControllerRef->GetPawn(), true);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

//Simple function to do a line trace.
//Takes an out parameter for the hitresult, and the start and end position of the line trace.
//Returns whether the line trace was successful or not.
bool ACustomGameMode::LineTrace(FHitResult& hitResult, FVector startPos, FVector endPos, ECollisionChannel collisionChannel) const
{
	//Draw debug line
	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, false, 0.2f);

	//Peform line trace using above params and values.
	return GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, collisionChannel);
}