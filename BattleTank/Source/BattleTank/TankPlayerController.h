// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Moving barrel towards crosshair
	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const;

	UFUNCTION()
		void OnTanksDeath();

	UFUNCTION()
		void OnAIDeath();

	virtual void SetPawn(APawn* InPawn) override;

	void CountAITanks();

	//Get all AIControllers and check if any of them got destroyed
	void GetAIControllers();

	//Return number of enemies left
	UFUNCTION(BlueprintCallable)
		int32 GetEnemiesLeft() const;

	//Change map to MainMenu
	void BackToMainMenu();

	UTankAimingComponent* AimingComponent = nullptr;

	//Crosshair
	UPROPERTY(EditAnywhere)
		float CrossHairXLocation = 0.5;

	UPROPERTY(EditAnywhere)
		float CrossHairYLocation = 0.33;

	UPROPERTY(EditAnywhere)
		float LineTraceRange = 1000000;

	UPROPERTY(EditDefaultsOnly)
		int32 NumberOfAI = 0;

	FTimerHandle OutTimerHandle;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
		void FoundAimingComponent(UTankAimingComponent* AimCompRef);
};
