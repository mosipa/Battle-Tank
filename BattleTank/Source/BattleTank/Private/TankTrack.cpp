// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto SlippedSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	//Correction to acceleration this frame
	auto CorrectionAcceleration = -SlippedSpeed / DeltaTime * GetRightVector();

	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;

	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	//Calculate force that will be applied on tracks
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	
	//Get location of track
	auto ForceLocation = GetComponentLocation();

	//Get tank body
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	
	if (!ensure(TankRoot)) { return; }

	TankRoot->AddForceAtLocation(
		ForceApplied,
		ForceLocation
	);
}


