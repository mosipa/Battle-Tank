// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Boost.h"
#include "TankBarrier.h"
#include "TankBarrierMesh.h"
#include "Classes/Engine/World.h"

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

	TArray<AActor*> SpawnedBarriers;
	UGameplayStatics::GetAllActorsOfClass(this, ATankBarrier::StaticClass(), SpawnedBarriers);

	for (auto Boost : SpawnedBoosts)
	{
		//TODO Doesnt work when 2 or more healthpacks have different values (only applies 1 value)
		BoostObject = Cast<ABoost>(Boost);
		HealthPackVal = BoostObject->GetHealthPackVal();
		BoostObject->BoostNotification.AddUniqueDynamic(this, &ATank::OnOverlappingBoost);
	}

	for (auto Barrier : SpawnedBarriers)
	{
		TankBarrier = Cast<ATankBarrier>(Barrier);
		TankBarrier->BarrierNotification.AddUniqueDynamic(this, &ATank::OnOverlappingBarrier);
	}
}

void ATank::OnOverlappingBarrier()
{
	BarriersLeft = FMath::Clamp<int32>(BarriersLeft + 1, 0, 3);
}

int32 ATank::GetBarriersLeft()
{
	return BarriersLeft;
}

void ATank::ActivateBarrier()
{
	if (BarriersLeft > 0)
	{
		auto TankBarrierMesh = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UTankBarrierMesh::StaticClass());
		if (!ensure(TankBarrierMesh)) { return; }
		
		Cast<UPrimitiveComponent>(TankBarrierMesh)->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Cast<USceneComponent>(TankBarrierMesh)->SetVisibility(true);
		UE_LOG(LogTemp, Warning, TEXT("Activating Barrier 1 out of %i"), BarriersLeft);
		BarriersLeft = FMath::Clamp<int32>(BarriersLeft - 1, 0, 3);
	}
	else
	{
		auto TankBarrierMesh = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UTankBarrierMesh::StaticClass());
		if (!ensure(TankBarrierMesh)) { return; }

		Cast<UPrimitiveComponent>(TankBarrierMesh)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Cast<USceneComponent>(TankBarrierMesh)->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("You dont have any barriers left"));
	}
}

void ATank::OnOverlappingBoost()
{
	//TODO bug with healing all spawned tanks instead of the one that is overlapping 
	//TODO possible another bug healing twice
	this->TanksCurrentHealth = FMath::Clamp<float>(this->TanksCurrentHealth + HealthPackVal, 0, TanksStartingHealth);
	UE_LOG(LogTemp, Warning, TEXT("Tank healed for %f with boost: %s"), BoostObject->GetHealthPackVal(), *BoostObject->GetName());
}
