// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingLevelScriptActor.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include "Actor_MoveAimTarget.h"

ATrainingLevelScriptActor::ATrainingLevelScriptActor()
{
	SetReplicates(true);
}

void ATrainingLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();


}

void ATrainingLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATrainingLevelScriptActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void ATrainingLevelScriptActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrainingLevelScriptActor, TargetMoveSpeed);
}

void ATrainingLevelScriptActor::SetTargetMoveSpeed(EMoveSpeed MoveSpeed)
{
	if (HasAuthority()) {
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("HasAuthority")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("HasAuthority X")));
	}

	TargetMoveSpeed = MoveSpeed;
	SetTargetMoveSpeedOnServer(MoveSpeed);
}

void ATrainingLevelScriptActor::SetTargetMoveSpeedOnClient_Implementation(EMoveSpeed MoveSpeed)
{
	TargetMoveSpeed = MoveSpeed;

	if (EMoveSpeed::Stop == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Set Level Stop")));
	}
	else if (EMoveSpeed::Run == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Set Level Run")));
	}
	else if (EMoveSpeed::Walk == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Set Level Walk")));
	}
	else if (EMoveSpeed::Random == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Set Level Random")));
	}
}

void ATrainingLevelScriptActor::SetTargetMoveSpeedOnServer_Implementation(EMoveSpeed MoveSpeed)
{
	TargetMoveSpeed = MoveSpeed;
	SetTargetMoveSpeedOnClient(MoveSpeed);
}


EMoveSpeed ATrainingLevelScriptActor::GetTargetMoveSpeed()
{
	if (EMoveSpeed::Stop == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Get Level Stop")));
	}
	else if (EMoveSpeed::Run == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Get Level Run")));
	}
	else if (EMoveSpeed::Walk == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Get Level Walk")));
	}
	else if (EMoveSpeed::Random == TargetMoveSpeed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Get Level Random")));
	}
	return TargetMoveSpeed;
}

TArray<FVector> ATrainingLevelScriptActor::GetRespawnLocs()
{
	return RespawnLocs;
}
