// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	auto Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s throttles %f"), *Name, Throttle);

	//Calculate force that will be applied on tracks
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	
	//Get location of track
	auto ForceLocation = GetComponentLocation();

	//Get tank body
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	
	TankRoot->AddForceAtLocation(
		ForceApplied,
		ForceLocation
	);
}


