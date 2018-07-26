// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

void UTankTurret::TurnTurret(float RelativeSpeed)
{
	//Clamping provided speed value
	auto ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, RelativeMinSpeed, RelativeMaxSpeed);

	//Rotate turret
	auto TurnChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;

	auto RawNewTurn = RelativeRotation.Yaw + TurnChange;

	SetRelativeRotation(FRotator(0.0, RawNewTurn, 0.0));
}


