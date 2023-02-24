// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndFlag.generated.h"

UCLASS()
class UE5_AR_API AEndFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBeginBox(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ShowFlag();

private:
	

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* boxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* mesh;

	class ACustomGameMode* gameModeRef;
};
