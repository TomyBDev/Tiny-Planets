// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityField.generated.h"

UCLASS()
class UE5_AR_API AGravityField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FVector GetGravityDirection(FVector playerPos) const;

	int GetPriority() const;

	void SetPriority(int level);

	virtual void SetDimensions(float radius);
	virtual void SetDimensions(float x, float y, float z, float scaleZ);

protected:
	UPROPERTY(EditInstanceOnly, Category="Settings")
	int priorityLevel = 1;
};
