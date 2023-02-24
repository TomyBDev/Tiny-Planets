// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityField.h"
#include "ParallelGravityField.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AParallelGravityField : public AGravityField
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AParallelGravityField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FVector GetGravityDirection(FVector playerPos) const override;

	virtual void SetDimensions(float x, float y, float z, float scaleZ);

private:
	UPROPERTY(VisibleAnywhere, Category="Component")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UArrowComponent* arrowComp;
};
