// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include "UserWidget_Crosshair.generated.h"

/**
 *
 */
UCLASS()
class MULTIFPS_API UUserWidget_Crosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetOpenAim();

	void DefaultAim();

	void SetAim();

	void SetHpbarPercent(float hpratio);

	void SetShieldbarPercent(float shieldratio);

	void SetSkill0_CoolTime(float curTime, float DefTime);

	void SetSkill1_CoolTime(float curTime, float DefTime);

	void GetFirstGun(FString GunName);

	void ThrowGun(int32 gunNum);

	void SelectFirstGun();

	void UpdateArmo(FString curArmo, int32 gunNum);

	void GetSecondGun(FString GunName);

	void SelectSecondGun();

private:
	UPROPERTY()
		class UImage* Image_UpAim;

	UPROPERTY()
		class UImage* Image_DownAim;

	UPROPERTY()
		class UImage* Image_LeftAim;

	UPROPERTY()
		class UImage* Image_RightAim;

	UPROPERTY()
		class UCanvasPanelSlot* ImageSlot_Right;

	UPROPERTY()
		class UCanvasPanelSlot* ImageSlot_Left;

	UPROPERTY()
		class UCanvasPanelSlot* ImageSlot_Up;

	UPROPERTY()
		class UCanvasPanelSlot* ImageSlot_Down;

	float DefaultDistance = 0;

	float MaXDistance = 100;

	float CurrentDistance;

	bool bOpenAim = false;

	UPROPERTY()
		class UProgressBar* Hpbar;

	UPROPERTY()
		class UProgressBar* Shieldbar;

	UPROPERTY()
		class UProgressBar* Skill0bar;

	UPROPERTY()
		class UProgressBar* Skill1bar;

	UPROPERTY()
		class UTextBlock* Skill0_CoolTime;

	UPROPERTY()
		class UTextBlock* Skill1_CoolTime;

	UPROPERTY()
		class UCanvasPanel* FirstGunPannel;
	UPROPERTY()
		class UCanvasPanel* SecondGunPannel;

	UPROPERTY()
		class UTextBlock* FirstGunName;
	UPROPERTY()
		class UTextBlock* SecondGunName;

	UPROPERTY()
		class UTextBlock* FirstGunArmo;
	UPROPERTY()
		class UTextBlock* SecondGunArmo;


};
