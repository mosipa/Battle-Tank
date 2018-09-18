// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boost.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoostDelegate);

class ATank;

UCLASS()
class BATTLETANK_API ABoost : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoost();

	FBoostDelegate BoostNotification;

private:
	UPROPERTY(VisibleAnywhere, Category = Component)
		UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Component)
		UStaticMeshComponent* BoostMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Setup)
		float Cooldown = 8.0f;

	float CooldownRemain = 0.0f;
	
	float LastTriggered = 0.0f;

	float YawPerSeconds = 36.0f;

	virtual void Tick(float DeltaTime) override;

	void OnOverlapByPlayer();

	void BoostMeshAnimation(float DeltaTime);
};
