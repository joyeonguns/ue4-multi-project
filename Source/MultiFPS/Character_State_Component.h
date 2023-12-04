// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character_State_Component.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDelicate_OneParam_float, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIFPS_API UCharacter_State_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacter_State_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FOnDelicate_OneParam_float OnDelicate_HpApply;
	FOnDelicate_OneParam_float OnDelicate_ShieldApply;
	FOnDelicate_OneParam_float OnDelicate_SpeedApply;

	void TakeDamage(float damage);

	void TakeDamageToShield(float damage);

	void TakeDamageToHp(float damage);

	// HP
	void SetHp(float newHp);
	UFUNCTION(unreliable, netmulticast)
		void SetHpOnClient(float newHp);
	UFUNCTION(unreliable, server)
		void SetHpOnServer(float newHp);

	// Shield
	void SetShield(float newShield);
	UFUNCTION(unreliable, netmulticast)
		void SetShieldOnClient(float newShield);
	UFUNCTION(unreliable, server)
		void SetShieldOnServer(float newShield);

	//Speed
	void SetSpeed(float newSpeed);

	TWeakObjectPtr<class AThirdPersonCharacter> player;


	float GetShield();
	float GetHp();


private:
	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
		float MaxHp = 100.0f;
	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
		float CurHp = 100.0f;

	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
		float MaxShield = 100.0f;
	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
		float CurShield = 100.0f;

	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
		float DefSpeed = 1.0f;
	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
		float CurSpeed = 1.0f;

	UPROPERTY(Replicated, EditAnywhere, Category = Stat)
	float ShieldRegenTime = 3.0f;
};
