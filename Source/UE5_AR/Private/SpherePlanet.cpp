// Fill out your copyright notice in the Description page of Project Settings.


#include "SpherePlanet.h"

#include "SphereGravityField.h"

// Sets default values
ASpherePlanet::ASpherePlanet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planetType = EPlanetTypeEnum::SpherePlanet;

}

// Called when the game starts or when spawned
void ASpherePlanet::BeginPlay()
{
	Super::BeginPlay();

	//Create the gravity sphere to go round planet and attach it to the planet.
	gravityField = GetWorld()->SpawnActor<ASphereGravityField>(ASphereGravityField::StaticClass(), GetActorLocation(), FRotator(0, 0, 0));
	gravityField->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	//Set the priority which was setup on this planet
	gravityField->SetPriority(priorityLevel);
	//Scale the gravity field to be bigger than the planet.
	float scale = mesh->GetRelativeScale3D().X;
	gravityField->SetDimensions(400 * scale);

}

// Called every frame
void ASpherePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}