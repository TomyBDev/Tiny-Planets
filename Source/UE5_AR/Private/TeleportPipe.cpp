// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPipe.h"

#include "Components/BoxComponent.h"
#include "PlayerPawn.h"
#include "CustomGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATeleportPipe::ATeleportPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComponent);

	boxComponent->SetGenerateOverlapEvents(true);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeleportPipe::OnOverlapBeginBox);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	mesh->SetupAttachment(boxComponent);

	TpToComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	TpToComp->SetupAttachment(boxComponent);
}

// Called when the game starts or when spawned
void ATeleportPipe::BeginPlay()
{
	Super::BeginPlay();
	
	AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld());
	gameModeRef = Cast<ACustomGameMode>(GM);
}

void ATeleportPipe::OnOverlapBeginBox(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		APlayerPawn* playerPawnRef = Cast<APlayerPawn>(OtherActor);
		if (IsValid(playerPawnRef) && IsValid(gameModeRef))
		{
			playerPawnRef->SetActorLocation(TpToComp->GetComponentLocation());
		}
	}
}

// Called every frame
void ATeleportPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}