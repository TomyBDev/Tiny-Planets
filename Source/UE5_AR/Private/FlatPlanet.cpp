// Fill out your copyright notice in the Description page of Project Settings.


#include "FlatPlanet.h"

#include "ParallelGravityField.h"

// Sets default values
AFlatPlanet::AFlatPlanet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planetType = EPlanetTypeEnum::FlatGround;

}

// Called when the game starts or when spawned
void AFlatPlanet::BeginPlay()
{
	Super::BeginPlay();

	//Create the gravity sphere to go round planet and attach it to the planet.
	gravityField = GetWorld()->SpawnActor<AParallelGravityField>(AParallelGravityField::StaticClass(), GetActorLocation(), GetActorRotation());
	gravityField->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	//Set the priority which was setup on this planet
	gravityField->SetPriority(priorityLevel);
	//Scale the gravity field to be bigger than the planet.
	FVector scale = mesh->GetRelativeScale3D();
	gravityField->SetDimensions(50 * scale.X, 50 * scale.Y, height, scale.Z);
}

// Called every frame
void AFlatPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}