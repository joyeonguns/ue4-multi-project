// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_MoveAimTarget.h"
#include "Net/UnrealNetwork.h"
#include "Math/RandomStream.h" 
#include "LevelActor_TrainningRoom.h"
#include "TrainingLevelScriptActor.h"

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


	bMoveRight = true;

	SetActorLocation(StartLoc);

	ChangeDirection();
	ChangeSpeed();
}

// Called every frame
void AActor_MoveAimTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRepeat(DeltaTime);
	
	ChangeSpeed();
}

void AActor_MoveAimTarget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AActor_MoveAimTarget::MoveRepeat(float DeltaTime)
{	
	if (MoveSpeed == EMoveSpeed::Stop) {
		ChangeSpeed();

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("MoveSpeed %f"), MoveSpeed));
		return;
	}

	if (FVector::Distance(GetActorLocation(), TargetLoc) >= 50.0f) {
		direction = (TargetLoc - GetActorLocation()).GetSafeNormal();
		FVector newLoc = GetActorLocation() + direction * curMoveSpeed * (DeltaTime / 1.0f);

		//SetActorLocation(newLoc);
		SetActorLocation(newLoc);
	}
	else {
		ChangeDirection();
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
	
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		MoveSpeed = LSA->GetTargetMoveSpeed();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("hm")));
	}

	switch (MoveSpeed)
	{
	case EMoveSpeed::Stop:
		curMoveSpeed = 0.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Stop %f"), curMoveSpeed));
		break;
	case EMoveSpeed::Walk:
		curMoveSpeed = 300.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Walk %f"), curMoveSpeed));
		break;
	case EMoveSpeed::Run:
		curMoveSpeed = 900.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Run %f"), curMoveSpeed));
		break;
	case EMoveSpeed::FastRun:
		curMoveSpeed = 1200.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("FastRun %f"), curMoveSpeed));
		break;
	case EMoveSpeed::Random:
		int32 rand = FMath::RandRange(1,4);
		curMoveSpeed = rand * 300.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Random %f"), curMoveSpeed));
		break;
	}
}


