// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//Start function called when game instance is created. Will load in saved data and store it in the variables below.
	virtual void OnStart() override;

	bool GetVibrate() const;

	bool GetMainMenu() const;

	void SetQuickRestart(bool b);

private:

	//Boolean to store if the game is in debug mode.
	UPROPERTY(BlueprintReadWrite, category = "Game Settings", meta = (AllowPrivateAccess = true))
		bool bDebugMode = false;

	UPROPERTY(BlueprintReadWrite, category = "Game Settings", meta = (AllowPrivateAccess = true))
		bool bMainMenu = true;

	//Boolean to store if game should show fps.
	UPROPERTY(BlueprintReadWrite, category = "Game Settings", meta = (AllowPrivateAccess = true))
		bool bShowFPS = false;

	//Boolean to store if phone should vibrate on damage taken.
	UPROPERTY(BlueprintReadWrite, category = "Game Settings", meta = (AllowPrivateAccess = true))
		bool bVibration = true;

	//Boolean to store if game should quick restart (no main menu).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Game Settings", meta = (AllowPrivateAccess = true))
		bool bQuickRestart = false;

	//Boolean to store float used for volume level.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Game Settings", meta = (AllowPrivateAccess = true))
		float volume = 0.75f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Game Settings", meta = (AllowPrivateAccess = true))
		int score = 0;

};
