// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

//Forward Declarations
class APlaceableActor;

UCLASS()
class UE5_AR_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual bool WorldHitTest(FHitResult& hitResult, FVector ScreenPos);

	void PlayerKilled();

	void SetGameWon();

	bool LineTrace(FHitResult& hitResult, FVector startPos, FVector endPos, ECollisionChannel collisionChannel) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetReadyToStart() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetGameStarted() const;

	UFUNCTION(BlueprintCallable)
	void SetGameStarted();

	UFUNCTION(BlueprintCallable)
	void SetWorldPlaced();

	UFUNCTION(BlueprintCallable)
	void PickupWorld();

private:

	enum class ESpawnStatus : uint8
	{
		NOTSPAWNED = 0 UMETA(DisplayName = "NotSpawned"),
		SPAWNED = 1  UMETA(DisplayName = "Spawned"),
		SPAWNLOCKED = 2  UMETA(DisplayName = "SpawnLocked"),
		PlanetType_Max UMETA(Hidden),
	};

	bool TrySpawnGameWorld(FVector WorldDir, FVector ScreenPos);

	UPROPERTY(EditAnywhere, Category = "References")
		USoundBase* ambienceSoundBase;

	UPROPERTY(EditAnywhere, Category = "References")
		USoundBase* musicSoundBase;

	UAudioComponent* ambienceRef;
	UAudioComponent* musicRef;

	class AHelloARManager* HelloARManager;

	class UARPin* worldPin;

	class ACustomGameStateBase* gameStateRef;
	class ACustomPlayerController* playerControllerRef;

	ULevelStreaming* levelRef;

	ESpawnStatus levelSpawnStatus = ESpawnStatus::NOTSPAWNED;

	bool bGameStarted = false;
};
