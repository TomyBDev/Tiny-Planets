// Fill out your copyright notice in the Description page of Project Settings.


#include "EndFlag.h"

#include "CustomGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"

// Sets default values
AEndFlag::AEndFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComponent);

	boxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEndFlag::OnOverlapBeginBox);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	mesh->SetupAttachment(boxComponent);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AEndFlag::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld());
	gameModeRef = Cast<ACustomGameMode>(GM);

	mesh->SetHiddenInGame(true);
	boxComponent->SetGenerateOverlapEvents(false);
}

// Called every frame
void AEndFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndFlag::ShowFlag()
{
	boxComponent->SetGenerateOverlapEvents(true);
	mesh->SetHiddenInGame(false);
	
}

void AEndFlag::OnOverlapBeginBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
	if (IsValid(OtherActor))
	{
		APlayerPawn* playerRef = Cast<APlayerPawn>(OtherActor);
		if (IsValid(playerRef) && IsValid(gameModeRef))
		{
			gameModeRef->SetGameWon();
		}
	}
}

