// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	//Clamp Relative Speed
	auto ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, MinRelativeSpeed, MaxRelativeSpeed);

	//Rotate barrel to given location
	auto ElevationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	auto ClampedElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(ClampedElevation, 0, 0));
}


