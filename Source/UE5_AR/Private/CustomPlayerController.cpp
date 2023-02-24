// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ACustomPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);


	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (IsValid(WinScreen))
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (IsValid(LoseScreen))
		{
			LoseScreen->AddToViewport();
		}
	}

}