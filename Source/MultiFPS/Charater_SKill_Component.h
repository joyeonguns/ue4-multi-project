// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Charater_SKill_Component.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDelicate_OneParam_float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDelicate_TwoParam, float, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIFPS_API UCharater_SKill_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharater_SKill_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnDelicate_TwoParam OnDelicate_UpdateSkill0CoolTime;
	FOnDelicate_TwoParam OnDelicate_UpdateSkill1CoolTime;

public:
	void InputSkill0();

	void InputSkill1();

	void ActionSkill0();

	void ActionSkill1();

	TWeakObjectPtr<class AThirdPersonCharacter> player;

private:
	float skill_Haste = 1;

	float cur_skill0_time;

	float cur_skill1_time;

	const float skill0_CoolTime = 10.0f;

	const float skill1_CoolTime = 10.0f;

	bool canSkill0 = true;

	bool canSkill1 = true;
};

