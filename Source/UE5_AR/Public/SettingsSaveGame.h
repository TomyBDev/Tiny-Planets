// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

    //Constructor

    USettingsSaveGame();

    // Save identifiers
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Identifier")
        FString SaveSlotName;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Identifier")
        int UserIndex;

    // Data to be saved.
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
        bool bDebugMode;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
        bool bShowFPS;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
        bool bVibrate;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
        float volume;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
        int score;
};
