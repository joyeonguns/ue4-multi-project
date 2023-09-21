// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Projectile.h"

// Sets default values
AActor_Projectile::AActor_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

