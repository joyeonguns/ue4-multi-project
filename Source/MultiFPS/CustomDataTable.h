// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CustomDataTable.generated.h"

USTRUCT(BlueprintType)
struct FGunData2 : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGunData2() : Armo(0), Damage(0), ShotTime(0), RelordTime(1) {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString GunName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Armo;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ShotTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float RelordTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Spread_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Spread_Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float CameraShake_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float CameraShake_Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AimPunch;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString GunType;
};

UCLASS()
class MULTIFPS_API ACustomDataTable : public AActor
{
	GENERATED_BODY()
	

};
