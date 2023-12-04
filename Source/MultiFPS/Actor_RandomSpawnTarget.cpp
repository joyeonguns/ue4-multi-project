// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_RandomSpawnTarget.h"

// Sets default values
AActor_RandomSpawnTarget::AActor_RandomSpawnTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
	BodyCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	BodyCollider->AttachTo(RootComponent);
	HeadCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Head"));
	HeadCollider->AttachTo(RootComponent);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AActor_RandomSpawnTarget::BeginPlay()
{
	Super::BeginPlay();
	

	FTimerHandle th;
	GetWorldTimerManager().SetTimer(th, this, &AActor_RandomSpawnTarget::DestroyOgj, 1.0f, false);
}

// Called every frame
void AActor_RandomSpawnTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_RandomSpawnTarget::DestroyOgj()
{
	Destroy();
}

