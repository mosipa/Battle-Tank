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
	
	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));
	Cast<USceneComponent>(Wheel)->SetupAttachment(Cast<USceneComponent>(MassWheelConstraint));
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	SetupConstraint();
}

void ASprungWheel::SetupConstraint()
{
	auto ParentActor = GetAttachParentActor();
	if (!ensure(ParentActor)) { return; }

	auto RootParentActor = ParentActor->GetRootComponent();
	if (!ensure(RootParentActor)) { return; }

	MassWheelConstraint->SetConstrainedComponents(
		Cast<UPrimitiveComponent>(RootParentActor),
		NAME_None,
		Cast<UPrimitiveComponent>(Wheel),
		NAME_None
	);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

