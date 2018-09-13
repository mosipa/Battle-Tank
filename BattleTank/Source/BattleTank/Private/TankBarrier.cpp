// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrier.h"
#include "Classes/Engine/World.h"
#include "Classes/Components/PrimitiveComponent.h"

// Sets default values
ATankBarrier::ATankBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMesh"));
	SetRootComponent(Cast<USceneComponent>(BaseMesh));
	Cast<USceneComponent>(BaseMesh)->SetVisibility(true);

	BoostMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BoostMesh"));
	Cast<USceneComponent>(BoostMesh)->SetVisibility(false);
}

// Called every frame
void ATankBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnOverlapByPlayer();
}

void ATankBarrier::OnOverlapByPlayer()
{
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerTank) { return; }
	if (!BoostMesh) { return; }

	bool bIsOverlapped = Cast<UPrimitiveComponent>(BoostMesh)->IsOverlappingActor(PlayerTank);

	CooldownRemain = GetWorld()->GetTimeSeconds() - LastTriggered;

	if (CooldownRemain > Cooldown)
	{
		//Activate
		Cast<USceneComponent>(BoostMesh)->SetVisibility(true);
		if (bIsOverlapped)
		{
			LastTriggered = GetWorld()->GetTimeSeconds();
			BarrierNotification.Broadcast();
		}
	}
	else
	{
		//Deactivate
		Cast<USceneComponent>(BoostMesh)->SetVisibility(false);
	}
}