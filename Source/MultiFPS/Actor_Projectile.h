// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "GameFramework/ProjectileMovementComponent.h"
#include "ActorComponent_FloatingDamage.h"
#include "Actor_Projectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileEnum : uint8
{
	LightShot UMETA(DisplayName = "LightShot"),
	HeavyShot UMETA(DisplayName = "HeavyShot"),
	SniperShot UMETA(DisplayName = "SniperShot"),
};

UENUM(BlueprintType)
enum class ETeamEnum : uint8
{
	RedTeam UMETA(DisplayName = "RedTeam"),
	BlueTeam UMETA(DisplayName = "BlueTeam"),
	NeutralityTeam UMETA(DisplayName = "NeutralityTeam"),
};

UCLASS()
class MULTIFPS_API AActor_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActor_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void Move(FVector dir);

	UFUNCTION()
		void OnRep_UpdateSetting();


public:
	UFUNCTION(unreliable, server)
		void SetActorLocationOnServer(FVector newLocation);

	UFUNCTION(unreliable, netmulticast)
		void SetActorLocationOnClient(FVector newLocation);

	UFUNCTION(unreliable, server)
		void DestroyObjectOnServer();

	UFUNCTION(unreliable, netmulticast)
		void DestroyObjectOnClient();

	UFUNCTION(unreliable, server)
		void HitOnServer(AActor* HitActor);

	UFUNCTION(unreliable, netmulticast)
		void HittOnClient(AActor* HitActor);

	UFUNCTION(reliable, netmulticast)
		void SpawnBulletHole(UPrimitiveComponent* OtherComponent, FVector ImpactPoint, FVector ImpactNormal);


	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting, EditAnywhere, Category = Gun)
		EProjectileEnum ProjectileType = EProjectileEnum::LightShot;


	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = MoveMent)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Mesh)
		class UStaticMeshComponent* StaticMesh;

	void Setloc(FVector loc);



	UFUNCTION(unreliable, netmulticast)
		void ShootOnClient(FVector dir);


	UFUNCTION(unreliable, server)
		void ShootOnServer(FVector dir);


	UFUNCTION(unreliable, netmulticast)
		void SpawnTextureOnMulti(int32 idx);

	void Fire(FVector dir);

	void FireDirection(FVector dir);

	UFUNCTION(unreliable, netmulticast)
	void FireDirectionOnServer(FVector dir);

	UFUNCTION(unreliable, server)
	void FireDirectionOnMulti(FVector dir);


	UPROPERTY(Replicated, EditAnywhere, Category=Team)
		ETeamEnum Team;

	void SetSetting(float _damage, FVector _dir, float _speed, EProjectileEnum _type, ETeamEnum _team, class AThirdPersonCharacter* _BulletOwner, float _AimPunch);

	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting)
	class AThirdPersonCharacter* BulletOwner;

private:

	UPROPERTY(EditAnywhere, Category = Component)
		class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = Component)
		class UMaterialInterface* Decal_Bullethole;

	UPROPERTY(EditAnywhere, Category = Niagara)
		class  UNiagaraSystem* NiagaraFX;


	class UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditAnywhere, Category = Damage)
		TArray<UTexture*> DamageFonts;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting, EditAnywhere, Category = Gun)
		float Damage = 10.0f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting, EditAnywhere, Category = Gun)
		float Speed = 100.0f;

	UPROPERTY(EditAnywhere, Category = Gun)
		float LifeTime = 55.0f;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting, EditAnywhere, Category = Gun)
		FVector MoveDirection;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting, EditAnywhere, Category = Gun)
		FVector Direction;

	bool bendMove = false;

	UPROPERTY(EditAnywhere, Category = Sound)
		class USoundBase* FireSound;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateSetting, EditAnywhere, Category = Gun)
	float AimPunch;
};
