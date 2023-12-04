// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_FloatingDamage.h"
#include "MyGameInstance.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UActorComponent_FloatingDamage::UActorComponent_FloatingDamage()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorComponent_FloatingDamage::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorComponent_FloatingDamage::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UActorComponent_FloatingDamage::SpawnDamageTextActorOnClient_Implementation(int32 _damage, FVector _spwLoc, int32 num, bool bCrit)
{
	FVector spwLoc = _spwLoc;
	FRotator spwRot;

	FActorSpawnParameters spwParam;

	if (GetWorld()) {
		auto DamageTextActorInstance = GetWorld()->SpawnActor<AActor_DamageText>(DamageTextActorClass, spwLoc, spwRot, spwParam);

		if (DamageTextActorInstance) {
			DamageTextActorInstance->SetDamageText(_damage, num, bCrit);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SPawnDamage"));
		}
	}


	auto GameInst = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInst) {
		GameInst->Damage += _damage;
	}
}


