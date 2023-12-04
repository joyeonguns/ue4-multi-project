// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake_Component.h"
#include "MyMatineeCameraShake.h"
#include "ThirdPersonCharacter.h"

// Sets default values for this component's properties
UCameraShake_Component::UCameraShake_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraShake_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCameraShake_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCameraShake_Component::ShakeCamera(float _duration, float _scale)
{
	ShakeCameraOnClient(_duration, _scale);
}

void UCameraShake_Component::ShakeCameraOnClient_Implementation(float _duration, float _scale)
{
	UMyMatineeCameraShake* CameraShake = NewObject<UMyMatineeCameraShake>();

	auto* owner = Cast<AThirdPersonCharacter>(GetOwner());
	if (owner == nullptr) return;


	APlayerController* TargetController = Cast<APlayerController>(owner->GetController());
	if (TargetController && CameraShake) {
		TargetController->ClientStartCameraShake(CameraShake->StaticClass(), _scale);
	}
}

