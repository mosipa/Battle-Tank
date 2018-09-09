// Fill out your copyright notice in the Description page of Project Settings.

#include "Boost.h"
#include "Tank.h"
#include "Classes/Engine/World.h"
#include "Classes/Components/PrimitiveComponent.h"

// Sets default values
ABoost::ABoost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoostMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BoostMesh"));
	SetRootComponent(Cast<USceneComponent>(BoostMesh));
}

// Called when the game starts or when spawned
void ABoost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnOverlapByPlayer();
}

void ABoost::OnOverlapByPlayer()
{
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerTank) { return; }
	if (!BoostMesh) { return; }

	bool bIsOverlapped = Cast<UPrimitiveComponent>(BoostMesh)->IsOverlappingActor(PlayerTank);

	if (bIsOverlapped)
	{
		BoostNotification.Broadcast();
	}
}

float ABoost::GetHealthPackVal()
{
	return HealthPackVal;
}