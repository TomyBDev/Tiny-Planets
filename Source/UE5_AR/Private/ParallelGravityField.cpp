// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallelGravityField.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AParallelGravityField::AParallelGravityField()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	arrowComp->SetupAttachment(BoxComponent);
	arrowComp->SetRelativeRotation(BoxComponent->GetUpVector().Rotation()*-1);
}

void AParallelGravityField::BeginPlay()
{
	Super::BeginPlay();
}

void AParallelGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AParallelGravityField::GetGravityDirection(FVector playerPos) const
{
	return arrowComp->GetForwardVector();
}

void AParallelGravityField::SetDimensions(float x, float y, float z, float scaleZ)
{

	BoxComponent->SetBoxExtent(FVector(x, y, z));
	BoxComponent->AddRelativeLocation(FVector(0, 0, z/scaleZ));
}
