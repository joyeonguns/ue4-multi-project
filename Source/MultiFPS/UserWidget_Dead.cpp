// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_Dead.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"

void UUserWidget_Dead::NativeConstruct()
{
	Super::NativeConstruct();

	Pannel_Main = Cast<UCanvasPanel>(GetWidgetFromName("CanvasPanel_Main"));
	TimerText = Cast<UTextBlock>(GetWidgetFromName("TextBlock_Timer"));
	RespawnBtn = Cast<UButton>(GetWidgetFromName("Button_Respawn"));
	RespawnBtn->OnClicked.AddDynamic(this, &UUserWidget_Dead::OnClick_Respawn);
}

void UUserWidget_Dead::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Timer -= InDeltaTime;


	if (Timer > 0) {
		if (TimerText) {
			FString time = FString::FromInt((int32)Timer);
			TimerText->SetText(FText::FromString(time));
		}

		if (RespawnBtn) {
			RespawnBtn->SetIsEnabled(false);
		}
	}
	else {
		Timer = 0.f;
		if (RespawnBtn) {
			RespawnBtn->SetIsEnabled(true);
		}
	}
	

}

void UUserWidget_Dead::OpenUI()
{
	Pannel_Main->SetVisibility(ESlateVisibility::Visible);
	Timer = 5.0f;
}

void UUserWidget_Dead::OnClick_Respawn()
{
	 auto plCont = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	 if (plCont) {
		 plCont->RespawnPlayer();
		 plCont->CloseRespawnUI();
	 }
}

void UUserWidget_Dead::CloseUI()
{
	Pannel_Main->SetVisibility(ESlateVisibility::Hidden);
}
