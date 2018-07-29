// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::AimAt(FVector HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	TankAimingComponent->SetBarrelReference(BarrelToSet);
	Barrel = BarrelToSet;
}

void ATank::SetTurretReference(UTankTurret* TurretToSet)
{
	TankAimingComponent->SetTurretReference(TurretToSet);
}

void ATank::Fire()
{
	if (!Barrel) { return; }

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
		if (!Projectile) { return; }
		Projectile->LaunchProjectile(LaunchSpeed);
	}
}