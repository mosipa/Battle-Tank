// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Elevate(float RelativeSpeed);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxDegreesPerSecond = 10.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxElevationDegrees = 10.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MinElevationDegrees = 0.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxRelativeSpeed = 1.0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MinRelativeSpeed = -1.0;
};
