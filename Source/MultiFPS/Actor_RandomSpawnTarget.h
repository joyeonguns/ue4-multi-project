// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Actor_RandomSpawnTarget.generated.h"

UCLASS()
class MULTIFPS_API AActor_RandomSpawnTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_RandomSpawnTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyOgj();
	
private:
	UPROPERTY(EditAnywhere, Category = Component)
		class USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, Category = Component)
		class UBoxComponent* BodyCollider;

	UPROPERTY(EditAnywhere, Category = Component)
		class UBoxComponent* HeadCollider;

	UPROPERTY(EditAnywhere, Category = Component)
	class UStaticMeshComponent* Mesh;
	
};
