// Fill out your copyright notice in the Description page of Project Settings.

#include "LightSwitchOn.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Gameframework/Pawn.h"
#include "Gameframework/PlayerController.h"

// Sets default values for this component's properties
ULightSwitchOn::ULightSwitchOn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightSwitchOn::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void ULightSwitchOn::LightSwitchOn()
{
	AActor* Owner = GetOwner();
	FRotator NewRotation = FRotator(0.0f, -180.0f, 0.0f);
	Owner->SetActorRotation(NewRotation);
	UE_LOG(LogTemp, Warning, TEXT("pressure plate triggered"));
}


// Called every frame
void ULightSwitchOn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))// if the actor that opens is in the volume
	{
		LightSwitchOn();
	}

}

