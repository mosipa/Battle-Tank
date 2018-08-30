// Fill out your copyright notice in the Description page of Project Settings.

#include "MortarAimingComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"

//TODO clean up code so it can shoot from 1,2,3 barrel turrets and with different cd on each barrel
void UMortarAimingComponent::Fire()
{
	if (!ensure(Barrel)) { return; }
	if (!ensure(ProjectileBlueprint)) { return; }

	//Setup parameters for spawn method
	auto OutProjectileSpawnLocation1 = Barrel->GetSocketLocation(FName("Projectile1"));
	auto OutProjectileSpawnRotation1 = Barrel->GetSocketRotation(FName("Projectile1"));

	auto OutProjectileSpawnLocation2 = Barrel->GetSocketLocation(FName("Projectile2"));
	auto OutProjectileSpawnRotation2 = Barrel->GetSocketRotation(FName("Projectile2"));

	auto OutProjectileSpawnLocation3 = Barrel->GetSocketLocation(FName("Projectile3"));
	auto OutProjectileSpawnRotation3 = Barrel->GetSocketRotation(FName("Projectile3"));

	//if Firing is off-cooldown
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		//Set cooldown on firing
		LastTimeFiredProjectile = GetWorld()->GetTimeSeconds();

		//Spawning projectile at socket location
		auto Projectile1 = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			OutProjectileSpawnLocation1,
			OutProjectileSpawnRotation1
			);

		auto Projectile2 = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			OutProjectileSpawnLocation2,
			OutProjectileSpawnRotation2
			);

		auto Projectile3 = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			OutProjectileSpawnLocation3,
			OutProjectileSpawnRotation3
			);

		//Launch projectile
		if (!ensure(Projectile1)) { return; }
		if (!ensure(Projectile2)) { return; }
		if (!ensure(Projectile3)) { return; }

		Projectile1->LaunchProjectile(LaunchSpeed);
		AmmoLeft--;

		Projectile2->LaunchProjectile(LaunchSpeed);
		AmmoLeft--;

		Projectile3->LaunchProjectile(LaunchSpeed);
		AmmoLeft--;
	}
}


