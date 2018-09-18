 // Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "HealthPack.h"
#include "TankBarrier.h"
#include "TankBarrierMesh.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);

	CountAITanks();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
	
	GetAIControllers(); 
	GetSpawnedBoosts();
}

void ATankPlayerController::CountAITanks()
{
	//Array for Tank actors spawned in level
	TArray<AActor*> TankArray;

	UGameplayStatics::GetAllActorsOfClass(this, ATankAIController::StaticClass(), TankArray);

	for (auto n : TankArray)
	{
		NumberOfAI++;
	}
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);

		if (!ensure(PossessedTank)) { return; }

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTanksDeath);
	}
}

void ATankPlayerController::GetAIControllers()
{
	//Array for AIControllers spawned in level
	TArray<AActor*> AIControllerArray;
	UGameplayStatics::GetAllActorsOfClass(this, ATankAIController::StaticClass(), AIControllerArray);

	for (auto AIController : AIControllerArray)
	{
		Cast<ATankAIController>(AIController)->OnAIDeath.AddUniqueDynamic(this, &ATankPlayerController::OnAIDeath);
	}
}

int32 ATankPlayerController::GetEnemiesLeft() const
{
	return NumberOfAI;
}

void ATankPlayerController::OnTanksDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("GameOver, you have been destroyed"));
	StartSpectatingOnly();

	GetWorldTimerManager().SetTimer(OutTimerHandle, this, &ATankPlayerController::BackToMainMenu, 10.0f, false);
}

void ATankPlayerController::OnAIDeath()
{
	NumberOfAI--;

	if (NumberOfAI == 0)
	{
		GetWorldTimerManager().SetTimer(OutTimerHandle, this, &ATankPlayerController::BackToMainMenu, 10.0f, false);
	}
}

void ATankPlayerController::BackToMainMenu()
{
	//TODO Add END_GAME level
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/_Levels/MainMenu"), TRAVEL_Absolute);
}

void ATankPlayerController::GetSpawnedBoosts()
{
	TArray<AActor*> SpawnedHealthPacks;
	UGameplayStatics::GetAllActorsOfClass(this, AHealthPack::StaticClass(), SpawnedHealthPacks);

	for (auto HealthP : SpawnedHealthPacks)
	{
		//TODO Doesnt work when 2 or more healthpacks have different values (only applies 1 value)
		HealthPack = Cast<AHealthPack>(HealthP);
		HealthPack->BoostNotification.AddUniqueDynamic(this, &ATankPlayerController::OnOverlappingBoost);
	}

	TArray<AActor*> SpawnedBarriers;
	UGameplayStatics::GetAllActorsOfClass(this, ATankBarrier::StaticClass(), SpawnedBarriers);

	for (auto Barrier : SpawnedBarriers)
	{
		TankBarrier = Cast<ATankBarrier>(Barrier);
		BarrierDuration = TankBarrier->GetBarrierDuration();
		TankBarrier->BoostNotification.AddUniqueDynamic(this, &ATankPlayerController::OnOverlappingBarrier);
	}
}

void ATankPlayerController::OnOverlappingBarrier()
{
	BarriersLeft = FMath::Clamp<int32>(BarriersLeft + 1, 0, 3);
}

int32 ATankPlayerController::GetBarriersLeft()
{
	return BarriersLeft;
}

void ATankPlayerController::ActivateBarrier()
{
	if (GetBarriersLeft() > 0)
	{
		auto TankBarrierMesh = GetPawn()->GetComponentByClass(UTankBarrierMesh::StaticClass());
		if (!ensure(TankBarrierMesh)) { return; }

		Cast<UPrimitiveComponent>(TankBarrierMesh)->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Cast<USceneComponent>(TankBarrierMesh)->SetVisibility(true);
		BarriersLeft = FMath::Clamp<int32>(BarriersLeft - 1, 0, 3);
		GetWorldTimerManager().SetTimer(OutTimerHandle, this, &ATankPlayerController::DeactivateBarrier, BarrierDuration, false);
	}
}

void ATankPlayerController::DeactivateBarrier()
{
	auto TankBarrierMesh = GetPawn()->GetComponentByClass(UTankBarrierMesh::StaticClass());
	if (!ensure(TankBarrierMesh)) { return; }

	Cast<UPrimitiveComponent>(TankBarrierMesh)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Cast<USceneComponent>(TankBarrierMesh)->SetVisibility(false);
}

void ATankPlayerController::OnOverlappingBoost()
{
	auto PlayerTank = Cast<ATank>(GetPawn());
	if (!ensure(PlayerTank)) { return; }
	if (!ensure(HealthPack)) { return; }

	auto HealthPackAmount = HealthPack->GetHealthPackValue();
	UE_LOG(LogTemp, Warning, TEXT("Added %f to tank %s"), HealthPackAmount,*(PlayerTank->GetName()));
	PlayerTank->SetTankHealth(HealthPackAmount);
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; }
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }
	
	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	//Size of player's viewport
	int32 OutViewportSizeX, OutViewportSizeY;
	GetViewportSize(OutViewportSizeX, OutViewportSizeY);

	//Crosshair location on player's viewport
	FVector2D ScreenLocation = FVector2D(OutViewportSizeX * CrossHairXLocation, OutViewportSizeY * CrossHairYLocation);
	
	//Deprojection
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility))
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	OutHitLocation = FVector(0.0);
	return false;
}