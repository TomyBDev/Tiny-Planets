// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"

#include "Components/BoxComponent.h"
#include "PlayerPawn.h"
#include "Components/InterpToMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ACollectable::ACollectable()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup a sphere which will be used for overlap checks.
	//It shall be the root component.
	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComponent);
	//Setup delegate functions.
	boxComponent->SetGenerateOverlapEvents(true);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnOverlapBeginSphere);
	//Setup enabling this in debug mode.
	boxComponent->bHiddenInGame = false;

	//Setup the skeletal mesh component.
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	mesh->SetupAttachment(boxComponent);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	interpToMovement = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpTo Movement Component"));
	interpToMovement->AddControlPointPosition(FVector(0, 0, 1));
	interpToMovement->AddControlPointPosition(FVector(0, 0, -1));
	interpToMovement->Duration = 1.f;
	interpToMovement->BehaviourType = EInterpToBehaviourType::PingPong;

	rotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	rotatingMovement->RotationRate = FRotator(0, 10, 0);
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectable::OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor))
	{
		APlayerPawn* player = Cast<APlayerPawn>(OtherActor);

		if (IsValid(player))
		{
			if (player->GetPowerUpTypeEnum() == EPowerUpTypeEnum::None || powerUpType == EPowerUpTypeEnum::Coin || powerUpType == EPowerUpTypeEnum::SpecialCoin)
			{
				//Call function handing in powerup type.
				player->SetPowerUpTypeEnum(powerUpType, powerUpDuration);
				Destroy();
			}

		}
	}

}

