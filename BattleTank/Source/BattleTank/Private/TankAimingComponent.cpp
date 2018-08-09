// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel && Turret)) { return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	//Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if(bHaveAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }
	//Difference between current barrel and turret rotation and desired aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();

	auto DeltaBarrelRotator = AimAsRotator - BarrelRotator;
	auto DeltaTurretRotator = AimAsRotator - TurretRotator;

	//Rotate barrel + turret to given location
	Barrel->Elevate(DeltaBarrelRotator.Pitch);
	Turret->TurnTurret(DeltaTurretRotator.Yaw);
}

void UTankAimingComponent::Fire()
{	
	if (!ensure(Barrel)) { return; }

	//Setup parameters for spawn method
	auto OutProjectileSpawnLocation = Barrel->GetSocketLocation(FName("Projectile"));
	auto OutProjectileSpawnRotation = Barrel->GetSocketRotation(FName("Projectile"));

	float CurrentTime = GetWorld()->GetTimeSeconds();

	//if Firing is off-cooldown
	if (CurrentTime - LastTimeFiredProjectile > FiringCooldown)
	{
		//Printing info about tank that is shooting
		auto TName = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Firing %s"), *(TName));

		//Set cooldown on firing
		LastTimeFiredProjectile = CurrentTime;

		//Spawning projectile at socket location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		OutProjectileSpawnLocation,
		OutProjectileSpawnRotation
		);

		//Launch projectile
		if (!ensure(Projectile)) { return; }
		Projectile->LaunchProjectile(LaunchSpeed);
	}
}