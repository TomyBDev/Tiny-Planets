// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockingCrate.h"

#include "Kismet/GameplayStatics.h"
#include "EnemyPawn.h"

// Sets default values
ABlockingCrate::ABlockingCrate()
{
	// Tick should be disabled.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(mesh);
}


void ABlockingCrate::BeginPlay()
{
	Super::BeginPlay();

	//Start a looping timer which calls the function which check if all enemies are dead.
	GetWorld()->GetTimerManager().SetTimer(conditionCheckTH, this, &ABlockingCrate::Action, checkTime, true);
}


void ABlockingCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABlockingCrate::Action()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), enemyClass, actors);

	for (AActor* actor : actors)
	{
		// Loop through all monster enemies, if any are alive just return.
		AEnemyPawn* enemyPawnRef = Cast<AEnemyPawn>(actor);
		if (IsValid(enemyPawnRef) && !enemyPawnRef->GetIsDead())
		{
			return;
		}
	}

	// No monster enemies alive remove crate.
	if (IsValid(crateBreakSound))
		UGameplayStatics::SpawnSound2D(GetWorld(), crateBreakSound);
	Destroy();
}