// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Actor_MoveAimTarget.h"
#include "TrainingLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class MULTIFPS_API ATrainingLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()


public:
	ATrainingLevelScriptActor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	void SetTargetMoveSpeed(EMoveSpeed MoveSpeed);

	UFUNCTION(reliable, netmulticast)
	void SetTargetMoveSpeedOnClient(EMoveSpeed MoveSpeed);

	UFUNCTION(reliable, server)
	void SetTargetMoveSpeedOnServer(EMoveSpeed MoveSpeed);

	EMoveSpeed GetTargetMoveSpeed();

	TArray<FVector> GetRespawnLocs();

private:
	UPROPERTY(Replicated)
	EMoveSpeed TargetMoveSpeed = EMoveSpeed::Walk;

	UPROPERTY(EditAnywhere, Category = SpwLoc)
	TArray<FVector> RespawnLocs;

};
