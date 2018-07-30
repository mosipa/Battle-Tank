// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	//Calculate force that will be applied on tracks
	float ClampedThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	auto ForceApplied = GetForwardVector() * ClampedThrottle * TrackMaxDrivingForce;
	
	//Get location of track
	auto ForceLocation = GetComponentLocation();

	//Get tank body
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	
	TankRoot->AddForceAtLocation(
		ForceApplied,
		ForceLocation
	);
}


