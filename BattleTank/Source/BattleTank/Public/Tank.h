// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup) //Change to EditDefaultsOnly when done testing
		float TanksStartingHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = Health)
		float TanksCurrentHealth = TanksStartingHealth;

	ATank();

	UFUNCTION(BlueprintPure, Category = Health)
		float GetHealthPercent() const;

public:
	// Sets default values for this pawn's properties
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	FTankDelegate OnDeath;
};
