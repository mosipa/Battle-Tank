// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	void TurnTurret(float RelativeSpeed);

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxDegreesPerSecond = 20.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float RelativeMaxSpeed = 1.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float RelativeMinSpeed = -1.0;
};
