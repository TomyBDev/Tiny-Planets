// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "EndFlag.h"

void ACustomGameStateBase::BeginPlay()
{
	Super::BeginPlay();

}

int ACustomGameStateBase::GetCoins() const
{
	return coins;
}

int ACustomGameStateBase::GetLives() const
{
	return lives;
}

int ACustomGameStateBase::GetKills() const
{
	return kills;
}

int ACustomGameStateBase::GetRedCoins() const
{
	return redCoins;
}

bool ACustomGameStateBase::GetShowSpeedLines() const
{
	return bShowSpeedLines;
}

void ACustomGameStateBase::AddCoins(int amountToAdd)
{
	coins += amountToAdd;
}

void ACustomGameStateBase::AddRedCoins(int amountToAdd)
{
	redCoins += amountToAdd;
	if (redCoins == 8)
	{
		AEndFlag* flag = Cast<AEndFlag>(UGameplayStatics::GetActorOfClass(GetWorld(), flagClass));
		if (IsValid(flag))
			flag->ShowFlag();
	}
}

void ACustomGameStateBase::AddLives(int amountToAdd)
{
	lives += amountToAdd;
}

void ACustomGameStateBase::AddKills(int amountToAdd)
{
	kills += amountToAdd;
}

void ACustomGameStateBase::setSpeedLines(bool b)
{
	bShowSpeedLines = b;
}
