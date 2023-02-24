// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlanet.h"

ABasePlanet::ABasePlanet()
{
	// Tick should be enabled.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(mesh);
}


void ABasePlanet::BeginPlay()
{
	Super::BeginPlay();
}


void ABasePlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


EPlanetTypeEnum ABasePlanet::GetPlanetType() const
{
	return planetType;
}