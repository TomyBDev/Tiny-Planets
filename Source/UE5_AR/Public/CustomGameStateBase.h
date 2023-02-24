// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API ACustomGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//Getters

	UFUNCTION(BlueprintPure, Category = "Getters")
	int GetCoins() const;

	UFUNCTION(BlueprintPure, Category = "Getters")
	int GetLives() const;

	UFUNCTION(BlueprintPure, Category = "Getters")
	int GetKills() const;

	UFUNCTION(BlueprintPure, Category = "Getters")
	int GetRedCoins() const;

	UFUNCTION(BlueprintPure, Category = "Getters")
	bool GetShowSpeedLines() const;

	//Adders (setters but += instead of =)

	void AddCoins(int amountToAdd);

	void AddRedCoins(int amountToAdd);

	void AddLives(int amountToAdd);

	void AddKills(int amountToAdd);

	void setSpeedLines(bool b);

private:

	UPROPERTY(EditAnywhere, category = "Game Settings", meta = (AllowPrivateAccess = true))
	int lives = 3;

	UPROPERTY(EditAnywhere, category = "Game Settings", meta = (AllowPrivateAccess = true))
	int kills = 0;

	bool bShowSpeedLines = false;

	int coins = 0;

	int redCoins = 0;


	UPROPERTY(EditAnywhere, category = "Reference", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AEndFlag> flagClass;
};
