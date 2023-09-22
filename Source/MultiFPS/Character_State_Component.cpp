// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_State_Component.h"
#include "MyMatineeCameraShake.h"
#include "ThirdPersonCharacter.h"

// Sets default values for this component's properties
UCharacter_State_Component::UCharacter_State_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void UCharacter_State_Component::BeginPlay()
{
	Super::BeginPlay();
	SetHp(100);
	SetShield(100);
	// ...

}


// Called every frame
void UCharacter_State_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (ShieldRegenTime >= 0) {
		ShieldRegenTime -= DeltaTime;
	}
	else {
		SetShield(CurShield + 20 * DeltaTime);
		if (CurShield >= 100) {
			ShieldRegenTime = 10.0f;
		}
	}

}

void UCharacter_State_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacter_State_Component, CurHp);
	DOREPLIFETIME(UCharacter_State_Component, CurShield);
	DOREPLIFETIME(UCharacter_State_Component, CurSpeed);
	DOREPLIFETIME(UCharacter_State_Component, MaxHp);
	DOREPLIFETIME(UCharacter_State_Component, MaxShield);
	DOREPLIFETIME(UCharacter_State_Component, DefSpeed);
}

void UCharacter_State_Component::TakeDamage(float damage)
{
	ShieldRegenTime = 3.0f;
	float SheildDamage = 0;
	float HpDamage = 0;

	SheildDamage = CurShield > damage ? damage : CurShield;
	HpDamage = damage - CurShield;

	TakeDamageToShield(SheildDamage);
	TakeDamageToHp(HpDamage);


	ShieldRegenTime = 10.0f;
}

void UCharacter_State_Component::TakeDamageToShield(float damage)
{
	if (damage > 0) {
		SetShield(CurShield - damage);
		OnDelicate_HpApply.Broadcast(CurHp / MaxHp);
	}
}

void UCharacter_State_Component::TakeDamageToHp(float damage)
{
	if (damage > 0) {

		SetHp(CurHp - damage);
		OnDelicate_ShieldApply.Broadcast(CurShield / MaxShield);

		if (player.IsValid()) {
			UMyMatineeCameraShake* CameraShake = NewObject<UMyMatineeCameraShake>();

			APlayerController* OwnerController = Cast<APlayerController>(player->GetController());
			if (OwnerController) {
				OwnerController->ClientStartCameraShake(CameraShake->StaticClass(), 0.5f);
			}
		}
	}

}

void UCharacter_State_Component::SetHp(float newHp)
{
	SetHpOnServer(newHp);
}

void UCharacter_State_Component::SetHpOnClient_Implementation(float newHp)
{
	CurHp = newHp;
	if (CurHp <= 0) {
		CurHp = 0;
	}
	else if (CurHp >= MaxHp) {
		CurHp = MaxHp;
	}

	if (player.IsValid() && !player->HasAuthority()) {
		OnDelicate_HpApply.Broadcast(CurHp / MaxHp);
	}
}

void UCharacter_State_Component::SetHpOnServer_Implementation(float newHp)
{
	SetHpOnClient(newHp);
}

void UCharacter_State_Component::SetShield(float newShield)
{
	SetShieldOnClient(newShield);
}

void UCharacter_State_Component::SetShieldOnClient_Implementation(float newShield)
{
	CurShield = newShield;
	if (CurShield <= 0) {
		CurShield = 0;
	}
	else if (CurShield >= MaxShield) {
		CurShield = MaxShield;
	}

	OnDelicate_ShieldApply.Broadcast(CurShield / MaxShield);
}

void UCharacter_State_Component::SetShieldOnServer_Implementation(float newShield)
{
	SetShieldOnClient(newShield);
}

void UCharacter_State_Component::SetSpeed(float newSpeed)
{
	CurSpeed = newSpeed;
}

