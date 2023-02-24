// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"

#include "SettingsSaveGame.h"
#include "kismet/GameplayStatics.h"

/// <summary>
/// Checks for a save file containing settings data. If a save slot is found, the data is loaded into variables.
/// </summary>
void UCustomGameInstance::OnStart()
{
	// Retrieve and cast the USaveGame object to USettingsSaveGame.
	if (USettingsSaveGame* LoadedGame = Cast<USettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SettingSaveSlot"), 0)))
	{
		// Save was found, so store the settings. 
		UE_LOG(LogTemp, Warning, TEXT("Save found loading settings"));
		bDebugMode = LoadedGame->bDebugMode;
		bShowFPS = LoadedGame->bShowFPS;
		bVibration = LoadedGame->bVibrate;
		volume = LoadedGame->volume;
		score = LoadedGame->score;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No Save file found."));
}

bool UCustomGameInstance::GetVibrate() const
{
	return bVibration;
}

bool UCustomGameInstance::GetMainMenu() const
{
	return bMainMenu;
}

void UCustomGameInstance::SetQuickRestart(bool b)
{
	bQuickRestart = b;
}
