// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor_TrainningRoom.h"
#include "Actor_RandomSpawnTimer.h"
#include <Kismet/GameplayStatics.h>

ALevelActor_TrainningRoom::ALevelActor_TrainningRoom()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelActor_TrainningRoom::BeginPlay()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_RandomSpawnTimer::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Actor->GetName());
		SpawnTimerActor = Cast<AActor_RandomSpawnTimer>(Actor);
		if (SpawnTimerActor) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("das"));
		}
		// "Actor"를 사용하여 필요한 작업을 수행합니다.
	}

	curTime = spwTime;

	

	//StartTraining();
}

void ALevelActor_TrainningRoom::Tick(float DeltaTime)
{
	
}

void ALevelActor_TrainningRoom::StartTraining()
{
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH, this, &ALevelActor_TrainningRoom::SpawnTrigerOn, 3.5f, false);
	if (SpawnTimerActor) {
		SpawnTimerActor->StartTimer(3.5f);
	}
}

void ALevelActor_TrainningRoom::SpawnTrigerOn()
{
	if (!bSpawn) {
		curTime = spwTime;
		bSpawn = true;
		GetWorldTimerManager().SetTimer(TH_target, this, &ALevelActor_TrainningRoom::SpawnTarget, interval, true);
		
		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH, this, &ALevelActor_TrainningRoom::SpawnTrigerOff, spwTime, false);

		if (SpawnTimerActor) {
			SpawnTimerActor->StartTimer(curTime);
		}
	}
}

void ALevelActor_TrainningRoom::SpawnTrigerOff()
{
	if (bSpawn) {
		bSpawn = false;
		GetWorldTimerManager().ClearTimer(TH_target);
	}
}

void ALevelActor_TrainningRoom::SpawnTarget()
{
	SpwLoc = FVector(FMath::RandRange(MaxLoc.X, MinLoc.X), FMath::RandRange(MaxLoc.Y, MinLoc.Y), FMath::RandRange(MaxLoc.Z, MinLoc.Z));
	FRotator spwRot = FVector(0, -90, 0).Rotation();
	FActorSpawnParameters spwParam;

	auto target = GetWorld()->SpawnActor<AActor_RandomSpawnTarget>(SpwTargetInstance, SpwLoc, spwRot, spwParam);
	if (target) {
		target->SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));
	}


}

void ALevelActor_TrainningRoom::SetTargetMoveSpeed(EMoveSpeed MoveSpeed)
{
	TargetMoveSpeed = MoveSpeed;
}

EMoveSpeed ALevelActor_TrainningRoom::GetTargetMoveSpeed()
{
	return TargetMoveSpeed;
}

void ALevelActor_TrainningRoom::SetTrainingLvl(int32 lvl)
{
	switch (lvl)
	{
	case 0: 
		interval = 1.5f;
		break;

	case 1:
		interval = 0.7f;
		break;

	case 2:
		interval = 0.5f;
		break;
	default:
		break;
	}
}
