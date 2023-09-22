// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SceneComponent.h>
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include "Actor_MoveAimTarget.generated.h"

UENUM(BlueprintType)
enum class EMoveSpeed : uint8
{
	Stop UMETA(DisplayName = "Stop"),
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	FastRun UMETA(DisplayName = "FastRun"),
	Random UMETA(DisplayName = "Random"),
};

UCLASS()
class MULTIFPS_API AActor_MoveAimTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActor_MoveAimTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void MoveRepeat(float DeltaTime);

	FVector RandomMove();

	FVector RandomSpeed();

	void ChangeDirection();
	void ChangeSpeed();

	UFUNCTION(reliable, server)
		void SetActorLocationOnServer(FVector newLoc);

	UFUNCTION(reliable, netmulticast)
		void SetActorLocationOnMulti(FVector newLoc);

private:
	UPROPERTY(EditAnywhere, Category = Move)
	EMoveSpeed MoveSpeed = EMoveSpeed::Walk;

	UPROPERTY(EditAnywhere, Category = Loc)
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category = Loc)
		class USphereComponent* HeadComponent;
	UPROPERTY(EditAnywhere, Category = Loc)
		class UBoxComponent* BodyComponent;
	UPROPERTY(EditAnywhere, Category = Loc)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated)
		float curMoveSpeed = 300.0f;

	UPROPERTY(Replicated)
		FVector direction;

	UPROPERTY(EditAnywhere, Category = Loc)
		float MoveDistance = 1450.f;

	UPROPERTY(Replicated)
		bool bMoveRight = true;

	UPROPERTY(Replicated)
		FVector DefLoc;

	UPROPERTY(Replicated)
		FVector MaxLoc;

	UPROPERTY(Replicated)
		FVector MinLoc;

	FVector TargetLoc;

	UPROPERTY(Replicated)
		bool RandMove = false;

	UPROPERTY(EditAnywhere, Category = Move)
	bool bRandomSpeed = false;
	UPROPERTY(EditAnywhere, Category = Move)
	bool bRandomDistance = false;

};
