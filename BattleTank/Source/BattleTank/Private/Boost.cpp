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

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMesh"));
	SetRootComponent(Cast<USceneComponent>(BaseMesh));
	Cast<USceneComponent>(BaseMesh)->SetVisibility(true);

	BoostMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BoostMesh"));
	Cast<USceneComponent>(BoostMesh)->SetVisibility(false);
}

// Called every frame
void ABoost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	BoostMeshAnimation(DeltaTime);
	OnOverlapByPlayer();
}

void ABoost::OnOverlapByPlayer()
{
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerTank) { return; }
	if (!ensure(BoostMesh)) { return; }

	bool bIsOverlapped = Cast<UPrimitiveComponent>(BoostMesh)->IsOverlappingActor(PlayerTank);
	
	CooldownRemain = GetWorld()->GetTimeSeconds() - LastTriggered;

	if (CooldownRemain > Cooldown)
	{
		//Activate
		Cast<USceneComponent>(BoostMesh)->SetVisibility(true);
		if (bIsOverlapped)
		{
			LastTriggered = GetWorld()->GetTimeSeconds();
			BoostNotification.Broadcast();
		}
	}
	else
	{
		//Deactivate
		Cast<USceneComponent>(BoostMesh)->SetVisibility(false);
	}
}

void ABoost::BoostMeshAnimation(float DeltaTime)
{
	//Rotate in Z axis
	float NewYaw = DeltaTime * YawPerSeconds;
	FRotator NewRotation = FRotator(0.0f, NewYaw, 0.0f);
	Cast<USceneComponent>(BoostMesh)->AddLocalRotation(NewRotation);

	//Move up and down
	float GameSeconds = GetWorld()->GetTimeSeconds();
	int32 intGameSeconds = GameSeconds;
	float UpDown = 0;

	if (intGameSeconds % 2 == 1)
	{
		UpDown = FMath::Pow((-1.0), 1);
	}
	else
	{
		UpDown = FMath::Pow((-1.0), 0);
	}

	FTransform NewTransform = FTransform(FVector(0.0f, 0.0f, UpDown * 1.0));
	Cast<USceneComponent>(BoostMesh)->AddLocalTransform(NewTransform);
}