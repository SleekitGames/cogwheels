// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Gameframework/Pawn.h"
#include "Gameframework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"

#define OUT //used to highlight variables which are changed by the function

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

///look for attached physics handle - if null, return error message to Log
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle ==nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing Component"), *GetOwner()->GetName())
	}
}

///look for attached Input Component, if not found, show error in Log
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) 
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("%s Missing Input Component"), *GetOwner()->GetName())
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (!PhysicsHandle) { return; }
	//if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) 
	{
		//move the object we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}


void UGrabber::Grab() {
		///line trace and try and reach any actors with physics channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto ActorHit = HitResult.GetActor();

		///if we hit something, attach a physics handle
		if (ActorHit)
		{
			if (!PhysicsHandle) { return; }
			PhysicsHandle->GrabComponentAtLocationWithRotation( //UE17 - changed from original
				ComponentToGrab,
				NAME_None, //no bones needed
				ComponentToGrab->GetOwner()->GetActorLocation(),
				FRotator(0) //allow rotation
			);
		}
}

void UGrabber::Release() {
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	///line trace (ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, 
		GetReachLineStart(), 
		GetReachLineEnd(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParameters
	);

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd() 
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * GrabberReach;
}


