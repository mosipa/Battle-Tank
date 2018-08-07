// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable)
		void Fire();

private:
	UPROPERTY(EditAnywhere, Category = Firing)
		float LaunchSpeed = 8000; //REMOVE after moving all code that needs it

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = Firing)
		float FiringCooldown = 5.0;

	//Reference to barrel for spawning projectile
	UTankBarrel* Barrel = nullptr;

	float LastTimeFiredProjectile = 0.0;
};
