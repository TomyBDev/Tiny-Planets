// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUpTypeEnum.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable.generated.h"

UCLASS()
class UE5_AR_API ACollectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Setting")
	EPowerUpTypeEnum powerUpType = EPowerUpTypeEnum::Star;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float powerUpDuration = 10.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* boxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UInterpToMovementComponent* interpToMovement;

	UPROPERTY(EditAnywhere, Category = "Components")
	class URotatingMovementComponent* rotatingMovement;
};
