// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Boost.h"

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

float ATank::SetTankHealth(float HealthPackAmount)
{
	return FMath::Clamp<float>(TanksCurrentHealth + HealthPackAmount, 0, TanksStartingHealth);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetSpawnedBoosts();
}

void ATank::GetSpawnedBoosts()
{
	TArray<AActor*> SpawnedBoosts;
	UGameplayStatics::GetAllActorsOfClass(this, ABoost::StaticClass(), SpawnedBoosts);

	for (auto Boost : SpawnedBoosts)
	{
		Cast<ABoost>(Boost)->BoostNotification.AddUniqueDynamic(this, &ATank::OnOverlappingBoost);
		UE_LOG(LogTemp, Warning, TEXT("%s name"), *(Boost->GetName()));
	}
}

void ATank::OnOverlappingBoost()
{
	UE_LOG(LogTemp, Warning, TEXT("Inside overlapping boost"));
	TanksCurrentHealth = FMath::Clamp<float>(TanksCurrentHealth + 20.0f, 0, TanksStartingHealth);
}
