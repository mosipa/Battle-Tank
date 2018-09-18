// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TanksCurrentHealth = TanksStartingHealth;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	FString TankName = GetName();

	UE_LOG(LogTemp, Warning, TEXT("%s received damage = %f"), *(TankName), DamageAmount);

	float DamageToApply = FMath::Clamp<float>(DamageAmount, 0, TanksCurrentHealth);

	TanksCurrentHealth -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Applied damage = %f to tank: %s"), DamageToApply, *(TankName));

	UE_LOG(LogTemp, Warning, TEXT("Tank %s HealthLeft = %f"), *(TankName), TanksCurrentHealth);

	if (TanksCurrentHealth == 0.f)
	{
		OnDeath.Broadcast();
	}

	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return TanksCurrentHealth / TanksStartingHealth;
}

void ATank::SetTankHealth(float HealthPackAmount)
{
	TanksCurrentHealth = FMath::Clamp<float>(TanksCurrentHealth + HealthPackAmount, 0, TanksStartingHealth);
}

float ATank::GetTanksCurrentHealth() const
{
	return TanksCurrentHealth;
}