// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityField.h"

// Sets default values
AGravityField::AGravityField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGravityField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AGravityField::GetGravityDirection(FVector playerPos) const
{
	return FVector();
}

int AGravityField::GetPriority() const
{
	return priorityLevel;
}

void AGravityField::SetPriority(int level)
{
	priorityLevel = level;
}

void AGravityField::SetDimensions(float radius)
{
}

void AGravityField::SetDimensions(float x, float y, float z, float scaleZ)
{
}

