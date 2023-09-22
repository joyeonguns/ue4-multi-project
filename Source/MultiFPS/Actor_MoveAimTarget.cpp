// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_MoveAimTarget.h"
#include "Net/UnrealNetwork.h"
#include "Math/RandomStream.h" 

// Sets default values
AActor_MoveAimTarget::AActor_MoveAimTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	HeadComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HeadComponent"));
	HeadComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	BodyComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyComponent"));
	BodyComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AActor_MoveAimTarget::BeginPlay()
{
	Super::BeginPlay();

	DefLoc = GetActorLocation();
	MaxLoc = GetActorLocation() + FVector(0, MoveDistance, 0);
	MinLoc = GetActorLocation() - FVector(0, MoveDistance, 0);

	if (HasAuthority()) {
		bMoveRight = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Max : %f,  Min : %f,  dis : %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	}
	else {
		bMoveRight = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Max : %f,  Min : %f,  dis : %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	
	}

	ChangeDirection();
	ChangeSpeed();
}

// Called every frame
void AActor_MoveAimTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRepeat(DeltaTime);
}

void AActor_MoveAimTarget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActor_MoveAimTarget, DefLoc);
	DOREPLIFETIME(AActor_MoveAimTarget, RandMove);
	DOREPLIFETIME(AActor_MoveAimTarget, curMoveSpeed);
	DOREPLIFETIME(AActor_MoveAimTarget, direction);
	DOREPLIFETIME(AActor_MoveAimTarget, MaxLoc);
	DOREPLIFETIME(AActor_MoveAimTarget, MinLoc);
	DOREPLIFETIME(AActor_MoveAimTarget, bMoveRight);
}

void AActor_MoveAimTarget::MoveRepeat(float DeltaTime)
{	
	if (FVector::Distance(GetActorLocation(), TargetLoc) >= 50.0f) {
		direction = (TargetLoc - GetActorLocation()).GetSafeNormal();
		FVector newLoc = GetActorLocation() + direction * curMoveSpeed * (DeltaTime / 1.0f);

		SetActorLocation(newLoc);
		SetActorLocationOnServer(newLoc);
	}
	else {
		ChangeDirection();
		ChangeSpeed();
	}
}

void AActor_MoveAimTarget::ChangeDirection()
{
	if (bMoveRight) {
		bMoveRight = false;
		TargetLoc = MaxLoc;
	}
	else {
		bMoveRight = true;
		TargetLoc = MinLoc;
	}

	if (bRandomDistance) {
		float rand = FMath::RandRange(MinLoc.Y, MaxLoc.Y);
		TargetLoc = FVector(TargetLoc.X, rand, TargetLoc.Z);
	}
}

void AActor_MoveAimTarget::ChangeSpeed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ChangeSpeed"));

	switch (MoveSpeed)
	{
	case EMoveSpeed::Stop:
		curMoveSpeed = 0.0f;
		break;
	case EMoveSpeed::Walk:
		curMoveSpeed = 300.0f;
		break;
	case EMoveSpeed::Run:
		curMoveSpeed = 600.0f;
		break;
	case EMoveSpeed::FastRun:
		curMoveSpeed = 900.0f;
		break;
	case EMoveSpeed::Random:
		int32 rand = FMath::RandRange(1,4);
		curMoveSpeed = rand * 300.0f;
		break;
	}
}
//
void AActor_MoveAimTarget::SetActorLocationOnServer_Implementation(FVector newLoc)
{
	SetActorLocationOnMulti(newLoc);
}

void AActor_MoveAimTarget::SetActorLocationOnMulti_Implementation(FVector newLoc)
{
	SetActorLocation(newLoc);
}

