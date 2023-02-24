// Fill out your copyright notice in the Description page of Project Settings.


#include "KillVolume.h"

#include "Components/BoxComponent.h"
#include "CharacterPawn.h"
// Sets default values
AKillVolume::AKillVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComponent);

	boxComponent->SetGenerateOverlapEvents(true);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlapBeginBox);

}

// Called when the game starts or when spawned
void AKillVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKillVolume::OnOverlapBeginBox(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		ACharacterPawn* charPawnRef = Cast<ACharacterPawn>(OtherActor);
		if (IsValid(charPawnRef))
		{
			charPawnRef->Kill();
		}
	}
}

// Called every frame
void AKillVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

