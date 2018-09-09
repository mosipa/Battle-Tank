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

	float GetHealthPackVal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	UPROPERTY(EditAnywhere, Category = Value)
		float HealthPackVal = 20.0f;

	UPROPERTY(VisibleAnywhere, Category = Component)
		UStaticMeshComponent* BoostMesh = nullptr;

	virtual void Tick(float DeltaTime) override;

	void OnOverlapByPlayer();
};
