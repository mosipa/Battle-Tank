// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankBarrier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBarrierDelegate);

UCLASS()
class BATTLETANK_API ATankBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankBarrier();

	FBarrierDelegate BarrierNotification;

private:
	UPROPERTY(VisibleAnywhere, Category = Component)
		UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Component)
		UStaticMeshComponent* BoostMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Value)
		float Cooldown = 2.0f;

	float CooldownRemain = 0.0f;

	float LastTriggered = 0.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnOverlapByPlayer();
};
