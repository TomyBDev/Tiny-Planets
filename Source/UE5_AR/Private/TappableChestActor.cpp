// Fill out your copyright notice in the Description page of Project Settings.


#include "TappableChestActor.h"

#include "Kismet/GameplayStatics.h"
#include "Collectable.h"

// Sets default values
ATappableChestActor::ATappableChestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATappableChestActor::BeginPlay()
{
	Super::BeginPlay();

}

void ATappableChestActor::Action()
{
	if (!IsValid(collectableRef))
	{
		FVector locToSpawn = GetActorLocation() + (GetActorRightVector() * 15.f) + (GetActorUpVector()* 5.f);
		FRotator rotToSpawn = FRotator(0, 0, 0);
		collectableRef = GetWorld()->SpawnActor<ACollectable>(collectableToSpawnClass, locToSpawn, rotToSpawn);
	}
}

// Called every frame
void ATappableChestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
