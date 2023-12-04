// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_RandomSpawnTimer.h"

// Sets default values
AActor_RandomSpawnTimer::AActor_RandomSpawnTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT"));
	RootComponent = TextRender;
}

// Called when the game starts or when spawned
void AActor_RandomSpawnTimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_RandomSpawnTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (curTime > 0) {
		curTime -= DeltaTime;
		SetText((int32)curTime);
	}

}

void AActor_RandomSpawnTimer::StartTimer(float time)
{
	curTime = time;
}

void AActor_RandomSpawnTimer::SetText(int32 num)
{
	FText text = FText::FromString(FString::FromInt(num));
	TextRender->SetText(text);
}

