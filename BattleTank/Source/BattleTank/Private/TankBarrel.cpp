// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	//Clamp Relative Speed
	auto ClampedRelativeSpeed = FMath::Clamp(RelativeSpeed, MinRelativeSpeed, MaxRelativeSpeed);

	//Rotate barrel/barrel + turret to given location
	auto ElevationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	//SetRelativeRotation(FRotator(RawNewElevation, 0, 0));

	auto ClampedElevation = FMath::Clamp(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(ClampedElevation, 0, 0));
}


