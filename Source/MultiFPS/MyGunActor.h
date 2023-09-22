// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <Components/SceneComponent.h>
#include "Net/UnrealNetwork.h"
#include "Actor_Projectile.h"
#include "Components/TimeLineComponent.h"
#include "MyGunActor.generated.h"

USTRUCT(BlueprintType)
struct FGunData
{
	GENERATED_BODY()

public:
	FGunData() : Armo(0), Damage(0), ShotTime(0), RelordTime(1) {};
	UPROPERTY(EditAnywhere, Category = "Data")
		int32 Armo;
	UPROPERTY(EditAnywhere, Category = "Data")
		float Damage;
	UPROPERTY(EditAnywhere, Category = "Data")
		float ShotTime;
	UPROPERTY(EditAnywhere, Category = "Data")
		float RelordTime;
	UPROPERTY(EditAnywhere, Category = "Data")
		float Spread_X;
	UPROPERTY(EditAnywhere, Category = "Data")
		float Spread_Y;
	UPROPERTY(EditAnywhere, Category = "Data")
		float Speed;
	UPROPERTY(EditAnywhere, Category = "Data")
		FString GunType;
};

UCLASS()
class MULTIFPS_API AMyGunActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyGunActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// getter setter
	class USkeletalMeshComponent* GetSK_Mesh();

	void SetOwner(AThirdPersonCharacter* _Owner);

	class AThirdPersonCharacter* Get_Owner();

	float Get_Damage();


	void Set_Equiped(bool _equip);

	bool Get_Equiped();

	UFUNCTION()
		void OnRep_UpdateEquip();

	UPROPERTY(ReplicatedUsing = OnRep_UpdateEquip, EditAnywhere, Category = bEquiped)
		bool bEquiped = false;

	int32 Get_C_Armo();

	void Set_C_Armo(int32 armo);

	void Set_curArmo(int32 armo);
	UFUNCTION(unreliable, netmulticast)
		void Set_curArmoOnMulti(int32 armo);
	UFUNCTION(unreliable, server)
		void Set_curArmoOnServer(int32 armo);

	int32 Get_M_Armo();

	void Set_M_Armo(int32 armo);

	UFUNCTION()
		void OnRep_UpdatedC_Armo();


	
	UFUNCTION()
		void OnRep_UpdatedMyOwner();

	void SetMyOwner(class AThirdPersonCharacter* _owner);


	void Equip(class AThirdPersonCharacter* _Owner);

	UFUNCTION(unreliable, server)
		void EquipOnServer(class AThirdPersonCharacter* _Owner);

	UFUNCTION(unreliable, netmulticast)
		void EquipOnMulti(class AThirdPersonCharacter* _Owner);

	FTimerHandle TH_AutoFire;

	void Fire(FVector dir);

	void AutoFire();

	void SingleFire();

	UFUNCTION(unreliable, server)
		void FireOnServer();

	void EndFire();

	UFUNCTION()
		void RecoilInputYaw(float x);
	UFUNCTION(unreliable, netmulticast)
		void RecoilInputYawOnMulti(float x);

	UFUNCTION()
		void RecoilInputPitch(float x);
	UFUNCTION(unreliable, netmulticast)
		void RecoilInputPitchOnMulti(float x);

	UFUNCTION()
		void RecoilFinish();
	void RecoilFinishOnMulti();

	UFUNCTION(unreliable, netmulticast)
		void ApplyRecoilOnMulti(FVector dir);


	UFUNCTION(unreliable, server)
		void ChargeArmoOnServer(int32 charge);

	UFUNCTION(unreliable, netmulticast)
		void ChargeArmoOnMulti(int32 charge);


	UPROPERTY(EditAnywhere, Category = Data)
		struct FGunData GunData;

	void SetSpawnLoc(FVector spwloc);

	bool bSpread = false;

	USkeletalMeshComponent* GetSKMesh();

	// Timeline
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess = "true"))
		UCurveFloat* YawCurve;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil", Meta = (AllowPrivateAccess = "true"))
		UCurveFloat* PitchCurve;

	UPROPERTY(Replicated)
		FTimeline RecoilTimeLine;

	// LerpTimeline
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
		float LerpTimelineLength;

	// TimelineComponent
	UTimelineComponent* MyTimeLineComp;


	UFUNCTION()
		void PlayRecoilTimeLine();
	UFUNCTION(unreliable, server)
		void PlayRecoilTimeLineOnServer();

	UPROPERTY(Replicated, EditAnywhere, Category = Armo)
		bool RecoilRecovery = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Mesh)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Component)
		class UBoxComponent* ItemBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Component)
		class USceneComponent* FirePoint;

	UPROPERTY(ReplicatedUsing = OnRep_UpdatedMyOwner, EditAnywhere, Category = Owner)
		class AThirdPersonCharacter* GunOwner = nullptr;

	UPROPERTY(EditAnywhere, Category = SPAWN)
		TSubclassOf<AActor_Projectile> projectile;

	UPROPERTY(EditAnywhere, Category = SPAWN)
		TSubclassOf<AActor_Projectile> Projectiles;

	float Damage;

	int32 M_Armo = 20;

	UPROPERTY(Replicated, EditAnywhere, Category = Armo)
		int32 C_Armo;

	float fireCooldown = 0;


	UPROPERTY(Replicated, EditAnywhere, Category = Armo)
		FVector SpawnLoc;

	UPROPERTY(Replicated, EditAnywhere, Category = Armo)
		FRotator OriginRotation;

	UPROPERTY(Replicated, EditAnywhere, Category = Armo)
		FRotator PostRecoilRotation;

};

