// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Actor_RandomSpawnTarget.h"
#include "Actor_RandomSpawnTimer.h"
#include "Actor_MoveAimTarget.h"
#include "LevelActor_TrainningRoom.generated.h"

/**
 * 
 */
UCLASS()
class MULTIFPS_API ALevelActor_TrainningRoom : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ALevelActor_TrainningRoom();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	void StartTraining();

	void SpawnTrigerOn();

	void SpawnTrigerOff();

	void SpawnTarget();

	void SetTargetMoveSpeed(EMoveSpeed MoveSpeed);

	EMoveSpeed GetTargetMoveSpeed();

	void SetTrainingLvl(int32 lvl);

private:
	FVector SpwLoc;

	UPROPERTY(EditAnywhere, Category = Location)
	FVector MaxLoc;

	UPROPERTY(EditAnywhere, Category = Location)
	FVector MinLoc;

	UPROPERTY(EditAnywhere, Category = Spawn)
	TSubclassOf<AActor_RandomSpawnTarget> SpwTargetInstance;

	UPROPERTY(EditAnywhere, Category = Spawn)
		float interval;

	bool bSpawn = false;

	float curTime;

	UPROPERTY(EditAnywhere, Category = Spawn)
	float spwTime;

	FTimerHandle TH_target;

	class AActor_RandomSpawnTimer* SpawnTimerActor;


	EMoveSpeed TargetMoveSpeed = EMoveSpeed::Stop;

};
