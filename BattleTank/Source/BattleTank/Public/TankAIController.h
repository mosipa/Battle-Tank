// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
class ATank;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	//Distance between actor and AI
	UPROPERTY(EditAnywhere, Category = Setup)
		float AcceptanceRadius = 3000;
};
