// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "BasePlanet.h"
#include "CustomGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CustomGameStateBase.h"
#include "Components/SphereComponent.h"


APlayerPawn::APlayerPawn()
{
	// Enabled tick.
	PrimaryActorTick.bCanEverTick = true;
}


void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APlayerPawn::NewTap(FVector impactPoint, AActor* actor)
{

	/**
	* If the planet tapped on is the same as the planet the player is standing on,
	* update the movement type and calcluated the target position (by offsetting is ply player radius),
	*/
	if (IsValid(groundRef) && IsValid(actor) && actor->GetName() == groundRef->GetName())
	{
		movementType = groundRef->GetPlanetType();

		switch (movementType)
		{
		case EPlanetTypeEnum::SpherePlanet:
			targetPos = impactPoint + ((impactPoint - actor->GetActorLocation()).GetSafeNormal() * sphereRadius);
			break;
		default:
			targetPos = impactPoint + (gravityDir * -sphereRadius);
			break;
		}

		bMoving = true;
	}
}


ABasePlanet* APlayerPawn::GetGroundReference() const
{
	return groundRef;
}


EPowerUpTypeEnum APlayerPawn::GetPowerUpTypeEnum() const
{
	return powerUp;
}


void APlayerPawn::SetPowerUpTypeEnum(EPowerUpTypeEnum powerUpType, float duration)
{
	switch (powerUpType)
	{
	case EPowerUpTypeEnum::None:
		// Do nothing
		break;

	case EPowerUpTypeEnum::Star:
		//make player yellow and play sound so they know they are powerful!
		powerUp = powerUpType;
		UGameplayStatics::PlaySound2D(GetWorld(), powerupSoundBase);
		if (IsValid(starMat))
			mesh->SetMaterial(0, starMat);
		break;

	case EPowerUpTypeEnum::Coin:
		// Don't update powerup type for coin, just increase counter in gamestate.
		if (IsValid(gamestateRef))
			gamestateRef->AddCoins(1);
		UGameplayStatics::PlaySound2D(GetWorld(), coinSoundBase);
		break;

	case EPowerUpTypeEnum::SpecialCoin:
		// Don't update powerup type for coin, just increase counter in gamestate.
		if (IsValid(gamestateRef))
			gamestateRef->AddCoins(5);
		UGameplayStatics::PlaySound2D(GetWorld(), coinSoundBase);
		break;

	case EPowerUpTypeEnum::SpeedUp:
		// Update boolean in gamestate which tracks if speed lines should be shown, and increase players speed.
		powerUp = powerUpType;
		speed = sprintSpeed;
		if (IsValid(gamestateRef))
			gamestateRef->setSpeedLines(true);
		UGameplayStatics::PlaySound2D(GetWorld(), powerupSoundBase);
		break;

	case EPowerUpTypeEnum::RedCoin:
		// Don't update powerup type for coin, just increase counter in gamestate.
		if (IsValid(gamestateRef))
			gamestateRef->AddRedCoins(1);
		UGameplayStatics::PlaySound2D(GetWorld(), coinSoundBase);
		break;

	default:
		// Do Nothing
		break;
	}


	// Only set power up time if its power up.
	if (powerUpType != EPowerUpTypeEnum::SpecialCoin && powerUpType != EPowerUpTypeEnum::Coin && powerUpType != EPowerUpTypeEnum::RedCoin)
		GetWorld()->GetTimerManager().SetTimer(powerUpTH, this, &APlayerPawn::EndPowerUp, duration);
}


void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//Set the spawn location for respawning the player.
	spawnLocation = GetActorLocation();

	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	gamestateRef = Cast<ACustomGameStateBase>(GS);
}


void APlayerPawn::Kill()
{
	// Parents kill should run first (important as it handles code that needs to be shared by the enemy and player)
	Super::Kill();

	if (!IsValid(gameModeRef))
		return;

	gameModeRef->PlayerKilled();

	// Just so the player doesn't get in the way some how. Probably not needed.
	SetActorLocation(FVector(-9999, 0, 0));

	// The player still has lives respawn them.
	if (gamestateRef->GetLives() > 0)
	{
		SetActorHiddenInGame(true);
		GetWorld()->GetTimerManager().SetTimer(respawnTH, this, &APlayerPawn::EndRespawn, respawnTime);
	}
}


void APlayerPawn::EndPowerUp()
{
	if (powerUp == EPowerUpTypeEnum::SpeedUp)
	{
		//Revert to walk speed and remove speedlines.
		speed = walkSpeed;
		if (IsValid(gamestateRef))
			gamestateRef->setSpeedLines(false);
	}

	if (powerUp == EPowerUpTypeEnum::Star)
	{
		//Revert the material
		if (IsValid(normalMat))
			mesh->SetMaterial(0, normalMat);
	}

	powerUp = EPowerUpTypeEnum::None;
}


void APlayerPawn::EndRespawn()
{
	// Reverts all the below values to what they were when the player was spawned.
	SetActorTickEnabled(true);
	SetActorLocation(spawnLocation);
	bDead = false;
	health = maxHealth;
	speed = walkSpeed;
	SetActorHiddenInGame(false);
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}
