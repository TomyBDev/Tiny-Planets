// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPawn.h"

#include "Kismet/GameplayStatics.h"
#include "GravityField.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"
#include "BasePlanet.h"
#include "CustomGameMode.h"
#include "Collectable.h"
#include "CustomGameInstance.h"


ACharacterPawn::ACharacterPawn()
{
	// Tick should be enabled.
	PrimaryActorTick.bCanEverTick = true;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComponent);

	// Setup collision for overlaps with gravity fields.
	sphereComponent->SetGenerateOverlapEvents(true);
	sphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Setup delegate functions.
	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACharacterPawn::OnOverlapBeginSphere);
	sphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACharacterPawn::OnOverlapEndSphere);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	mesh->SetupAttachment(sphereComponent);
}


void ACharacterPawn::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;
	speed = walkSpeed;
	sphereRadius = sphereComponent->GetUnscaledSphereRadius();

	gravityDir = -GetActorUpVector();
	movementDir = GetActorForwardVector();

	// Get the highest priority gravity field and stores a reference to it.
	GetNewGravField();

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	gameModeRef = Cast<ACustomGameMode>(GM);

	UGameInstance* GI = GetWorld()->GetGameInstance();
	gameInstanceRef = Cast<UCustomGameInstance>(GI);
}


void ACharacterPawn::Kill()
{
	bDead = true;
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SetActorTickEnabled(false);
}


void ACharacterPawn::Tick(float DeltaTime)
{
	// Call parents tick.
	Super::Tick(DeltaTime);

	// Calc gravity vector and if the player should rotate.
	Gravity();

	// Moves and Rotates the character.
	MoveCharacter(DeltaTime);
}


bool ACharacterPawn::GetIsDead() const
{
	return bDead;
}


bool ACharacterPawn::GetIsFalling() const
{
	// Getter is opposite of the bool as it makes more sense for Animation Blueprint.
	return !bGrounded;
}


float ACharacterPawn::GetSpeed() const
{
	// the length of the velocity is the scalar speed in the direction horizontal to the ground.
	return velocity.Length();
}


FVector ACharacterPawn::GetGravityDirection() const
{
	return gravityDir;
}

float ACharacterPawn::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	health -= DamageAmount;

	// Vibration on damage taken. (only gets called on player atm so its here).
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) && IsValid(gameInstanceRef) && gameInstanceRef->GetVibrate())
		PC->PlayDynamicForceFeedback(0.5f, 0.5f, true, true, true, true, EDynamicForceFeedbackAction::Start);

	if (health <= 0.f)
	{
		//Set the character in dead state.
		Kill();
	}

	return DamageAmount;
}


void ACharacterPawn::OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If the character enters a new gravity field, and there isn't already a higher priority gravity field
	//set the current gravity field to the new one.
	AGravityField* gravField = Cast<AGravityField>(OtherActor);
	if (IsValid(gravField))
	{
		if (IsValid(currentfield))
		{
			if (currentfield->GetPriority() < gravField->GetPriority())
				currentfield = gravField;
		}
		else
			currentfield = gravField;
	}
}


void ACharacterPawn::OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//If the character left the gravity field that is currently in effect,
	//set the pointer to null and find the next gravity field with the highest priority.
	if (OtherActor && currentfield && OtherActor->GetUniqueID() == currentfield->GetUniqueID())
	{
		currentfield = nullptr;
		GetNewGravField();
	}
}


void ACharacterPawn::MoveCharacter(float dt)
{
	//Get new character forward vector, where the character is right side up in terms of gravity.
	//And is still facing in the same forward direction. This will be overwritten if the character is moving...
	//... horizontally (i.e. moving towards tap location or player).
	movementDir = GetActorRightVector().Cross(-gravityDir);

	if (bMoving)
	{
		//If the target has been reached
		if (GetActorLocation().Equals(targetPos, 5.f))
		{
			bMoving = false;
			velocity = { 0,0,0 };
		}
		else
		{
			FVector temp;
			switch (movementType)
			{
			case EPlanetTypeEnum::SpherePlanet:
				// Calculates the direction of movement on a spherical planet.
				movementDir = (targetPos - GetActorLocation()).GetSafeNormal();
				temp = movementDir.Cross(-gravityDir);
				movementDir = temp.Cross(gravityDir);
				break;

			default:
				// Calculates the movement direction on non-spherical planet (currently only flat planet).
				movementDir = (targetPos - GetActorLocation()).GetSafeNormal();
				break;
			}

			velocity = movementDir * speed;
		}
	}

	//Check if there is free space in from of the character to move, if there isn't space set horizontal movement to zero.
	ForwardCheck(dt);

	//Combine horizontal movement and gravitation movement.
	AddActorWorldOffset((velocity + gravity) * dt);

	//Calculate the target rotation of the character based on the direction of movement and the gravity direction.
	const FRotator characterDir = UKismetMathLibrary::MakeRotFromXZ(movementDir, -gravityDir);

	//Character only rotates if in a short range of planet or is moving on ground (i.e. walking).
	if (bShouldRotate || bMoving)
	{
		//Interpolate between current rotation and target rotation
		FQuat rot = FMath::QInterpConstantTo(GetActorQuat(), characterDir.Quaternion(), dt, 7.5f);

		SetActorRotation(rot);
	}
}


void ACharacterPawn::Gravity()
{
	if (IsValid(currentfield))
	{
		gravityDir = currentfield->GetGravityDirection(GetActorLocation());

		// If the character is not on the ground, and is within a set range of the ground, indicate that the character should begin to rotate to the correct orientation.
		if (!bGrounded)
		{
			FHitResult hitResult;
			const FVector start = GetActorLocation();
			const FVector end = start + (gravityDir * flipHeight);
			bShouldRotate = gameModeRef->LineTrace(hitResult, start, end, ECC_WorldDynamic);
		}

		//Increase the gravity (The float here is the gravity amount in m/s/s)
		gravity += gravityDir * gravityStrength;
	}
	else //If there is no gravity scale just go down in the characters current rotation
		gravityDir = -GetActorUpVector();

	//Check if the player is on the ground.
	GroundCheck();

}


void ACharacterPawn::GroundCheck()
{
	//LineTrace immediately below the character. If it hits the player is grounded.
	FHitResult hitResult;
	const FVector start = GetActorLocation();
	const FVector end = start + (gravityDir * sphereRadius);
	bGrounded = gameModeRef->LineTrace(hitResult, start, end, ECC_WorldDynamic);

	//If the character is on the ground, zero the gravity and adjust the position of the character back above ground.
	if (bGrounded)
	{
		gravity = { 0,0,0 };
		SetActorLocation(hitResult.ImpactPoint + (gravityDir * (- sphereRadius+1)));

		ABasePlanet* ground = Cast<ABasePlanet>(hitResult.GetActor());
		if (IsValid(ground))
		{
			groundRef = ground;
			return;
		}
		
		groundRef = nullptr;
		return;
	}

	//The character is not on the ground so update relevant variables
	groundRef = nullptr;
	bMoving = false;
	velocity = { 0,0,0 };
}


void ACharacterPawn::GetNewGravField()
{
	// Get all the overlapping gravity fields
	TArray<AActor*> actors;
	sphereComponent->GetOverlappingActors(actors);

	// Loop through all the gravity fields and set the new current gravity field.
	for (AActor* a : actors)
	{
		AGravityField* gravField = Cast<AGravityField>(a);
		if (IsValid(gravField))
		{
			// If currently not in a gravity field, store the new gravity field.
			// Otherwise check if the new gravity field has higher priority than the current one, if so store it instead.
			if (!IsValid(currentfield))
				currentfield = gravField;
			else if (currentfield->GetPriority() < gravField->GetPriority())
				currentfield = gravField;
		}
	}
}


void ACharacterPawn::ForwardCheck(float dt)
{
	//Does a sweep in the forward direction of the character using a sphere the same size as the hitbox of the character.
	//If there is a collision (that is not a collectable) stop character movement in the horizontal plane.
	FCollisionShape shape = FCollisionShape::MakeSphere(sphereRadius - 2);
	FHitResult hitResult;

	if (GetWorld()->SweepSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + (velocity * dt), FQuat(0, 0, 0, 0), ECC_WorldDynamic, shape))
	{
		if (IsValid(hitResult.GetActor()) && !IsValid(Cast<ACollectable>(hitResult.GetActor())))
		{
			velocity = { 0,0,0 };
			bMoving = false;
		}
	}
}