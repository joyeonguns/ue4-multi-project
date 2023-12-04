// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_State_Component.h"
#include "MyMatineeCameraShake.h"
#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>
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


	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Beginplay : ")));
	SetHp(70);
	SetShield(90);
	// ...
}


// Called every frame
void UCharacter_State_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CurShield < 100) {
		if (ShieldRegenTime > 0) {
			ShieldRegenTime -= DeltaTime;
		}
		else {
			ShieldRegenTime = 0;
			SetShield(CurShield + 5 * (DeltaTime / 1));
		}
	}
	else {

	}

	SetShield(CurShield);
	

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
	DOREPLIFETIME(UCharacter_State_Component, ShieldRegenTime);
}

void UCharacter_State_Component::TakeDamage(float damage)
{
	ShieldRegenTime = 5.0f;
	float SheildDamage = 0;
	float HpDamage = 0;

	if (CurShield > damage) {
		// 쉴드만 데미지
		SheildDamage = damage;
		TakeDamageToShield(SheildDamage);
	}
	else {
		SheildDamage = CurShield;
		HpDamage = damage - CurShield;

		TakeDamageToShield(SheildDamage);
		TakeDamageToHp(HpDamage);
	}



}

void UCharacter_State_Component::TakeDamageToShield(float damage)
{
	if (damage > 0) {
		SetShield(CurShield - damage);
	}
}

void UCharacter_State_Component::TakeDamageToHp(float damage)
{
	if (damage > 0) {

		SetHp(CurHp - damage);
		OnDelicate_HpApply.Broadcast(CurHp / MaxHp);

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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetName() + FString::Printf(TEXT(" Die")));			
		
		if (player.IsValid()) {
			player->PlayerDeath();
		}
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

	OnDelicate_ShieldApply.Broadcast(CurShield / MaxShield);

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

	//OnDelicate_ShieldApply.Broadcast(CurShield / MaxShield);

}

void UCharacter_State_Component::SetShieldOnServer_Implementation(float newShield)
{
	SetShieldOnClient(newShield);
}

void UCharacter_State_Component::SetSpeed(float newSpeed)
{
	CurSpeed = newSpeed;
}

float UCharacter_State_Component::GetShield()
{
	return CurShield;
}

float UCharacter_State_Component::GetHp()
{
	return CurHp;
}


