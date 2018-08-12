// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrack();

	UFUNCTION(BlueprintCallable, Category = Input)
		void SetThrottle(float Throttle);

	//Max force per track in Newton
	UPROPERTY(EditDefaultsOnly)
		float TrackMaxDrivingForce = 2000000; //Tank mass (40 t) and acceleration (10 m/s^2)

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
};
