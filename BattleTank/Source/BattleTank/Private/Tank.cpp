// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
}

void ATank::Fire()
{
	if (!ensure(Barrel)) { return; }

	//Setup parameters for spawn method
	auto OutProjectileSpawnLocation = Barrel->GetSocketLocation(FName("Projectile"));
	auto OutProjectileSpawnRotation = Barrel->GetSocketRotation(FName("Projectile"));

	float CurrentTime = GetWorld()->GetTimeSeconds();

	//if Firing is off-cooldown
	if (CurrentTime - LastTimeFiredProjectile > FiringCooldown)
	{
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