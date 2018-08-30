// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankAimingComponent.h"
#include "MortarAimingComponent.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UMortarAimingComponent : public UTankAimingComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void Fire() override;
};
