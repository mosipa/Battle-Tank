// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boost.h"
#include "TankBarrier.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBarrierDelegate);

UCLASS()
class BATTLETANK_API ATankBarrier : public ABoost
{
	GENERATED_BODY()
	
public:	
	float GetBarrierDuration() const;

private:
	UPROPERTY(EditAnywhere, Category = Setup)
		float BarrierDuration = 4.0f;

};
