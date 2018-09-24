// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	MassAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassAxleConstraint"));
	SetRootComponent(Cast<USceneComponent>(MassAxleConstraint));
	
	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Cast<USceneComponent>(Axle)->SetupAttachment(Cast<USceneComponent>(MassAxleConstraint));

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Cast<USceneComponent>(Axle));

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Cast<USceneComponent>(Wheel)->SetupAttachment(Cast<USceneComponent>(AxleWheelConstraint));
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraint();
}

void ASprungWheel::SetupConstraint()
{
	auto ParentActor = GetAttachParentActor();
	if (!ensure(ParentActor)) { return; }

	auto RootParentActor = ParentActor->GetRootComponent();
	if (!ensure(RootParentActor)) { return; }

	MassAxleConstraint->SetConstrainedComponents(
		Cast<UPrimitiveComponent>(RootParentActor),
		NAME_None,
		Cast<UPrimitiveComponent>(Axle),
		NAME_None
	);

	AxleWheelConstraint->SetConstrainedComponents(
		Cast<UPrimitiveComponent>(Axle),
		NAME_None,
		Cast<UPrimitiveComponent>(Wheel),
		NAME_None
	);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0;
	}
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}


void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplyForce();
}