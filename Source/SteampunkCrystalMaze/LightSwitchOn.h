// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "LightSwitchOn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLightEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STEAMPUNKCRYSTALMAZE_API ULightSwitchOn : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULightSwitchOn();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FLightEvent OnPress;

	UPROPERTY(BlueprintAssignable)
		FLightEvent OnRelease;

private:
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY(EditAnywhere)
		float TriggerMass = 1.f;

	// The owning light
	AActor* Owner = nullptr;

	float GetTotalMassOfActorsOnPlate();
};
