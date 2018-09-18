// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boost.h"
#include "HealthPack.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API AHealthPack : public ABoost
{
	GENERATED_BODY()
	
public:
	float GetHealthPackValue() const;

private:
	UPROPERTY(EditAnywhere, Category = Setup)
		float HealthPackValue = 20.0f;
	
};
