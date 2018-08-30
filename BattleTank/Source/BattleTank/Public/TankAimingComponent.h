// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "TankAimingComponent.generated.h"

//Possible states of aiming component
UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable)
		virtual	void Fire();

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = Setup)
		virtual void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = Firing)
		int32 GetAmmoLeft() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = State)
		EFiringState FiringState = EFiringState::Reloading;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float LaunchSpeed = 8000;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		int32 AmmoLeft = 5; //Find default value

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	float LastTimeFiredProjectile = 0.0;

private:
	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float FiringCooldown = 5.0;

	FVector LastBarrelLocation = FVector(0);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	virtual void BeginPlay() override;

	void MoveBarrelTowards(FVector AimDirection);

	bool IsBarrelMoving();

	//Get FiringState to change color of aim point in BP
	EFiringState GetCurrentFiringState();
};