// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereGravityField.h"

#include "Components/SphereComponent.h"

ASphereGravityField::ASphereGravityField()
{
	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComponent->SetupAttachment(RootComponent);
}

void ASphereGravityField::BeginPlay()
{
	Super::BeginPlay();
}

void ASphereGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASphereGravityField::GetGravityDirection(FVector playerPos) const
{
	FVector vector1 = sphereComponent->GetCenterOfMass() - playerPos;
	FVector direction; float size;
	vector1.ToDirectionAndLength(direction, size);
	return direction;
}

void ASphereGravityField::SetDimensions(float radius)
{
	sphereComponent->SetSphereRadius(radius);
}
