// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/TextRenderComponent.h>
#include "Actor_RandomSpawnTimer.generated.h"

UCLASS()
class MULTIFPS_API AActor_RandomSpawnTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_RandomSpawnTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void StartTimer(float time);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void SetText(int32 num);

private:
	UPROPERTY(EditAnywhere, Category = Component)
	class UTextRenderComponent* TextRender;

	float curTime;
};
