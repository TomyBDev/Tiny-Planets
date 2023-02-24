// Fill out your copyright notice in the Description page of Project Settings.


#include "TappableActor.h"

// Sets default values
ATappableActor::ATappableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(mesh);
}

// Called when the game starts or when spawned
void ATappableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATappableActor::Action()
{



}

// Called every frame
void ATappableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

