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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastTimeFiredProjectile = GetWorld()->GetTimeSeconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GetCurrentFiringState();
}

EFiringState UTankAimingComponent::GetCurrentFiringState()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (AmmoLeft == 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if (CurrentTime - LastTimeFiredProjectile < FiringCooldown)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
	return FiringState;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetAmmoLeft() const
{
	return AmmoLeft;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	FVector BarrelLocation = Barrel->GetForwardVector();

	//Barrel hasn't moved
	if (BarrelLocation.Equals(LastBarrelLocation, 0.001)) 
	{ 
		return false;
	}
	//Barrel moved
	else 
	{ 
		LastBarrelLocation = BarrelLocation; 
		return true; 
	}
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

	if (FMath::Abs(DeltaTurretRotator.Yaw) < 180)
	{
		Turret->TurnTurret(DeltaTurretRotator.Yaw);
	}
	else
	{
		Turret->TurnTurret(-DeltaTurretRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{	
	if (!ensure(Barrel)) { return; }
	if (!ensure(ProjectileBlueprint)) { return; }

	//Setup parameters for spawn method
	auto OutProjectileSpawnLocation = Barrel->GetSocketLocation(FName("Projectile"));
	auto OutProjectileSpawnRotation = Barrel->GetSocketRotation(FName("Projectile"));

	//if Firing is off-cooldown
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		//Set cooldown on firing
		LastTimeFiredProjectile = GetWorld()->GetTimeSeconds();

		//Spawning projectile at socket location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		OutProjectileSpawnLocation,
		OutProjectileSpawnRotation
		);

		//Launch projectile
		if (!ensure(Projectile)) { return; }

		Projectile->LaunchProjectile(LaunchSpeed);
		AmmoLeft--;
	}
}