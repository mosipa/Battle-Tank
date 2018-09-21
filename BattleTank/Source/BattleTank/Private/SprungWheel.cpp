// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(Cast<USceneComponent>(MassWheelConstraint));

	Mass = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mass"));
	Cast<USceneComponent>(Mass)->SetupAttachment(Cast<USceneComponent>(MassWheelConstraint));
	
	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));
	Cast<USceneComponent>(Wheel)->SetupAttachment(Cast<USceneComponent>(MassWheelConstraint));
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetAttachParentActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not null: %s") ,*(GetAttachParentActor()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Null"));
	}
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

