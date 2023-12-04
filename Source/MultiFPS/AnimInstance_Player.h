// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyGunActor.h"
#include "AnimInstance_Player.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE(FOnDelicate_NoParam);

UCLASS()
class MULTIFPS_API UAnimInstance_Player : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstance_Player();
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	FOnDelicate_NoParam OnDelicate_GunReload_Start;
	FOnDelicate_NoParam OnDelicate_GunReload_End;

	FOnDelicate_NoParam OnDelicate_StartChangeGun;
	FOnDelicate_NoParam OnDelicate_EndChangeGun;
	FOnDelicate_NoParam OnDelicate_ChangeGun;


	FOnDelicate_NoParam OnDelicate_ReloadArmoOut;
	FOnDelicate_NoParam OnDelicate_ReloadArmoIn;
	FOnDelicate_NoParam OnDelicate_ReloadAction;

protected:

private:

	UFUNCTION()
		void AnimNotify_ReloadStart();

	UFUNCTION()
		void AnimNotify_ReloadEnd();

	UFUNCTION()
		void AnimNotify_StartChangeGun();

	UFUNCTION()
		void AnimNotify_EndChangeGun();

	UFUNCTION()
		void AnimNotify_ChangeGun();


	UFUNCTION()
		void AnimNotify_Reload_ArmoOut();
	UFUNCTION()
		void AnimNotify_Reload_ArmoIn();
	UFUNCTION()
		void AnimNotify_Reload_Action();


};

