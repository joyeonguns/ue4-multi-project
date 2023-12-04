// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Player.h"

UAnimInstance_Player::UAnimInstance_Player()
{

}

void UAnimInstance_Player::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}

void UAnimInstance_Player::AnimNotify_ReloadStart()
{
	OnDelicate_GunReload_Start.Broadcast();
}

void UAnimInstance_Player::AnimNotify_ReloadEnd()
{
	OnDelicate_GunReload_End.Broadcast();
}

void UAnimInstance_Player::AnimNotify_StartChangeGun()
{
	OnDelicate_StartChangeGun.Broadcast();
}

void UAnimInstance_Player::AnimNotify_EndChangeGun()
{
	OnDelicate_EndChangeGun.Broadcast();
}

void UAnimInstance_Player::AnimNotify_ChangeGun()
{
	OnDelicate_ChangeGun.Broadcast();
}

void UAnimInstance_Player::AnimNotify_Reload_ArmoOut()
{
	OnDelicate_ReloadArmoOut.Broadcast();
}

void UAnimInstance_Player::AnimNotify_Reload_ArmoIn()
{
	OnDelicate_ReloadArmoIn.Broadcast();
}

void UAnimInstance_Player::AnimNotify_Reload_Action()
{
	OnDelicate_ReloadAction.Broadcast();
}
