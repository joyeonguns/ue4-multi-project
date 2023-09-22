// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_Crosshair.h"

void UUserWidget_Crosshair::NativeConstruct()
{
	Super::NativeConstruct();

	Image_UpAim = Cast<UImage>(GetWidgetFromName(TEXT("UpAim")));
	Image_DownAim = Cast<UImage>(GetWidgetFromName(TEXT("DownAim")));
	Image_LeftAim = Cast<UImage>(GetWidgetFromName(TEXT("leftAim")));
	Image_RightAim = Cast<UImage>(GetWidgetFromName(TEXT("RightAim")));

	ImageSlot_Up = Cast<UCanvasPanelSlot>(Image_UpAim->Slot);

	ImageSlot_Down = Cast<UCanvasPanelSlot>(Image_DownAim->Slot);

	ImageSlot_Left = Cast<UCanvasPanelSlot>(Image_LeftAim->Slot);

	ImageSlot_Right = Cast<UCanvasPanelSlot>(Image_RightAim->Slot);


	Hpbar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_HpBar")));
	Shieldbar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_ShieldBar")));

	Skill0bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill0_bar")));
	Skill1bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill1_bar")));

	Skill0_CoolTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("Skill0CoolTime")));
	Skill1_CoolTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("Skill1CoolTime")));


	FirstGunPannel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Weapon1_Pannel")));
	FirstGunName = Cast<UTextBlock>(GetWidgetFromName(TEXT("GunName_1")));
	FirstGunArmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("Armo_1")));
	FirstGunPannel->SetVisibility(ESlateVisibility::Hidden);

	SecondGunPannel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Weapon2_Pannel")));
	SecondGunName = Cast<UTextBlock>(GetWidgetFromName(TEXT("GunName_2")));
	SecondGunArmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("Armo_2")));
	SecondGunPannel->SetVisibility(ESlateVisibility::Hidden);
}

void UUserWidget_Crosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bOpenAim && CurrentDistance < MaXDistance) {
		CurrentDistance += InDeltaTime * MaXDistance * 5;
		SetAim();
	}
}

void UUserWidget_Crosshair::SetOpenAim()
{
	if (!bOpenAim)
		bOpenAim = true;
}

void UUserWidget_Crosshair::DefaultAim()
{
	CurrentDistance = DefaultDistance;
	bOpenAim = false;
	SetAim();
}

void UUserWidget_Crosshair::SetAim()
{
	if (ImageSlot_Up) {
		ImageSlot_Up->SetPosition(FVector2D(0, CurrentDistance * -1));
	}

	if (ImageSlot_Down) {
		ImageSlot_Down->SetPosition(FVector2D(0, CurrentDistance));
	}

	if (ImageSlot_Left) {
		ImageSlot_Left->SetPosition(FVector2D(CurrentDistance, 0));
	}

	if (ImageSlot_Right) {
		ImageSlot_Right->SetPosition(FVector2D(CurrentDistance * -1, 0));
	}
}

void UUserWidget_Crosshair::SetHpbarPercent(float hpratio)
{
	if (Hpbar) {
		Hpbar->SetPercent(hpratio);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hpbar Error"));


}

void UUserWidget_Crosshair::SetShieldbarPercent(float shieldratio)
{
	if (Shieldbar) {
		Shieldbar->SetPercent(shieldratio);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Shield Error"));

}

void UUserWidget_Crosshair::SetSkill0_CoolTime(float curTime, float DefTime)
{
	float rate = 1 - curTime / DefTime;

	if (Skill0bar) {
		Skill0bar->SetPercent(rate);
	}

	FString rateText = FString::Printf(TEXT("%.1f"), curTime);

	if (Skill0_CoolTime) {
		Skill0_CoolTime->SetText(FText::FromString(rateText));
		if (curTime == 0) {
			Skill0_CoolTime->SetText(FText::FromString(" "));
		}
	}
}

void UUserWidget_Crosshair::SetSkill1_CoolTime(float curTime, float DefTime)
{
	float rate = 1 - curTime / DefTime;

	if (Skill1bar) {
		Skill1bar->SetPercent(rate);
	}

	FString rateText = FString::Printf(TEXT("%.1f"), curTime);

	if (Skill1_CoolTime) {
		Skill1_CoolTime->SetText(FText::FromString(rateText));
		if (curTime == 0) {
			Skill1_CoolTime->SetText(FText::FromString(" "));
		}
	}
}

void UUserWidget_Crosshair::GetFirstGun(FString GunName)
{
	if (FirstGunPannel) {
		FirstGunPannel->SetVisibility(ESlateVisibility::Visible);
	}

	if (FirstGunName) {
		FirstGunName->SetText(FText::FromString(GunName));
	}
}

void UUserWidget_Crosshair::ThrowGun(int32 GunNum)
{
	if (GunNum == 0) {
		if (FirstGunPannel) {
			FirstGunPannel->SetVisibility(ESlateVisibility::Hidden);
		}

		if (FirstGunName) {
			FirstGunName->SetText(FText::FromString(""));
		}
	}
	else if (GunNum == 1) {
		if (SecondGunPannel) {
			SecondGunPannel->SetVisibility(ESlateVisibility::Hidden);
		}

		if (SecondGunName) {
			SecondGunName->SetText(FText::FromString(""));
		}
	}

}

void UUserWidget_Crosshair::SelectFirstGun()
{
	if (FirstGunPannel) {
		FirstGunPannel->SetVisibility(ESlateVisibility::Visible);
	}

	if (SecondGunPannel) {
		SecondGunPannel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUserWidget_Crosshair::UpdateArmo(FString curArmo, int32 gunNum)
{
	if (gunNum == 0) {
		if (FirstGunArmo) {
			FirstGunArmo->SetText(FText::FromString(curArmo));
		}
	}
	else if (gunNum == 1) {
		if (SecondGunArmo) {
			SecondGunArmo->SetText(FText::FromString(curArmo));
		}
	}
}

void UUserWidget_Crosshair::GetSecondGun(FString GunName)
{
	if (SecondGunPannel) {
		SecondGunPannel->SetVisibility(ESlateVisibility::Visible);
	}

	if (FirstGunName) {
		FirstGunName->SetText(FText::FromString(GunName));
	}
}


void UUserWidget_Crosshair::SelectSecondGun()
{
	if (FirstGunPannel) {
		FirstGunPannel->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SecondGunPannel) {
		SecondGunPannel->SetVisibility(ESlateVisibility::Visible);
	}
}

