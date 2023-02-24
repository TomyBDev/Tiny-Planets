// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPawn.h"

#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlanet.h"
#include "CustomGameStateBase.h"

AEnemyPawn::AEnemyPawn()
{
	// Tick should be enabled.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(playerRef))
	{
		// Check if enemy should be moving towards the player.
		bMoving = perceptionCheck();

		if (bMoving)
		{
			if (bFleeing)
			{
				// If fleeing from the player get a vector in the opposite direction
				FVector dir = (GetActorLocation() - playerRef->GetActorLocation()).GetSafeNormal();
				targetPos = GetActorLocation() + (dir * 100);
			}
			else // Is not fleeing so just set the target position as the players position.
				targetPos = playerRef->GetActorLocation();
		}
		return;
	}

	GetPlayerRef();
}

bool AEnemyPawn::perceptionCheck()
{
	ABasePlanet* playerGroundRef = playerRef->GetGroundReference();

	if (!IsValid(playerRef))
		return false;

	if (playerRef->GetIsDead())
		return false;

	// If the player is on the same planet as the enemy
	if (IsValid(playerGroundRef) && IsValid(groundRef) && playerGroundRef->GetUniqueID() == groundRef->GetUniqueID())
	{
		float dist = (playerRef->GetActorLocation() - GetActorLocation()).Length();

		if (dist < 200.f)
		{
			// If in range of player, set moving. If player has star power, set fleeing true.
			if (playerRef->GetPowerUpTypeEnum() == EPowerUpTypeEnum::Star)
				bFleeing = true;
			else
				bFleeing = false;

			return true;
		}
	}
	return false;
}

bool AEnemyPawn::GetAttacking() const
{
	return bAttacking;
}

void AEnemyPawn::SetAttacking(bool b)
{
	bAttacking = b;
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	//Get and store player reference
	GetPlayerRef();
}

void AEnemyPawn::OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Calls characters implementation of OnOverlapBegin (handles gravity field interactions)
	Super::OnOverlapBeginSphere(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!IsValid(playerRef) || !IsValid(OtherActor))
		return;

	// If the player collided with is
	if (playerRef->GetUniqueID() == OtherActor->GetUniqueID())
	{
		if (playerRef->GetPowerUpTypeEnum() == EPowerUpTypeEnum::Star)
		{
			//Kills the enemy is the player has star power.
			AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
			ACustomGameStateBase* gameStateRef = Cast<ACustomGameStateBase>(GS);
			if (IsValid(gameStateRef))
				gameStateRef->AddKills(1);

			Kill();
			return;
		}

		// Player doesn't have star power, attack the player. And start a looping attack timer.
		AttackPlayer();
		GetWorld()->GetTimerManager().SetTimer(attackTH, this, &AEnemyPawn::AttackPlayer, attackRate, true);
	}

}

void AEnemyPawn::OnOverlapEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEndSphere(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	// if stopped overlapping with player cancel the looping attack timer.
	if (playerRef->GetUniqueID() == OtherActor->GetUniqueID())
	{
		GetWorld()->GetTimerManager().ClearTimer(attackTH);
	}
}

void AEnemyPawn::GetPlayerRef()
{
	// Get and store reference to player pawn.
	AActor* actor = UGameplayStatics::GetActorOfClass(this, playerClass);
	if (actor && Cast<APlayerPawn>(actor))
	{
		playerRef = Cast<APlayerPawn>(actor);
	}
}

void AEnemyPawn::AttackPlayer()
{
	if (!IsValid(playerRef))
		return;

	if (playerRef->GetIsDead())
	{
		// If the player is dead stop the looping attack timer.
		GetWorld()->GetTimerManager().ClearTimer(attackTH);
		return;
	}

	//Set attacking so animation blueprint knows to play attack animation, and inform player of damage dealt.
	bAttacking = true;
	playerRef->TakeDamage(25.f, FDamageEvent(), GetInstigatorController(), this);
}
