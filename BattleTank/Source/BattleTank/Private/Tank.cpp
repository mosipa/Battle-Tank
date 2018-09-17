// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
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

void ATank::SetTankHealth(float HealthPackAmount)
{
	TanksCurrentHealth = FMath::Clamp<float>(TanksCurrentHealth + HealthPackAmount, 0, TanksStartingHealth);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetSpawnedBoosts();
}

void ATank::GetSpawnedBoosts()
{
	TArray<AActor*> SpawnedBarriers;
	UGameplayStatics::GetAllActorsOfClass(this, ATankBarrier::StaticClass(), SpawnedBarriers);

	for (auto Barrier : SpawnedBarriers)
	{
		TankBarrier = Cast<ATankBarrier>(Barrier);
		BarrierDuration = TankBarrier->GetBarrierDuration();
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
		BarriersLeft = FMath::Clamp<int32>(BarriersLeft - 1, 0, 3);
		GetWorldTimerManager().SetTimer(OutTimerHandle, this, &ATank::DeactivateBarrier, BarrierDuration, false);
	}
}

void ATank::DeactivateBarrier()
{
	auto TankBarrierMesh = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UTankBarrierMesh::StaticClass());
	if (!ensure(TankBarrierMesh)) { return; }

	Cast<UPrimitiveComponent>(TankBarrierMesh)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Cast<USceneComponent>(TankBarrierMesh)->SetVisibility(false);
}

float ATank::GetTanksCurrentHealth() const
{
	return TanksCurrentHealth;
}