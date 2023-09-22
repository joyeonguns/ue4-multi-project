// Fill out your copyright notice in the Description page of Project Settings.


#include "Charater_SKill_Component.h"

// Sets default values for this component's properties
UCharater_SKill_Component::UCharater_SKill_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharater_SKill_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCharater_SKill_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!canSkill0) {
		cur_skill0_time -= (DeltaTime * skill_Haste);
		if (cur_skill0_time <= 0) {
			cur_skill0_time = 0;
			canSkill0 = true;
		}
		OnDelicate_UpdateSkill0CoolTime.Broadcast(cur_skill0_time, skill0_CoolTime);
	}

	if (!canSkill1) {
		cur_skill1_time -= (DeltaTime * skill_Haste);

		if (cur_skill1_time <= 0) {
			cur_skill1_time = 0;
			canSkill1 = true;
		}
		OnDelicate_UpdateSkill1CoolTime.Broadcast(cur_skill1_time, skill1_CoolTime);
	}

}

void UCharater_SKill_Component::InputSkill0()
{
	if (canSkill0) {
		ActionSkill0();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Skill0 is CoolTime"));
	}
}

void UCharater_SKill_Component::InputSkill1()
{
	if (canSkill1) {
		ActionSkill1();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Skill1 is CoolTime"));
	}
}

void UCharater_SKill_Component::ActionSkill0()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ActionSkill0"));

	canSkill0 = false;
	cur_skill0_time = skill0_CoolTime;
}

void UCharater_SKill_Component::ActionSkill1()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ActionSkill1"));

	canSkill1 = false;
	cur_skill1_time = skill1_CoolTime;
}

