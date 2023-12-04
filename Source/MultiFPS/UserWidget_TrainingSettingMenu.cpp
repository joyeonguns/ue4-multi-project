// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_TrainingSettingMenu.h"
#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "ThirdPersonCharacter.h"
#include "MyGunActor.h"
#include "MyGameInstance.h"
#include "TrainingLevelScriptActor.h"

void UUserWidget_TrainingSettingMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	Pannel_Main = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Main")));

	// TopButton
	Btn_Setting = Cast<UButton>(GetWidgetFromName(TEXT("Button_Setting")));
	Btn_Setting->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Setting);

	Btn_Game = Cast<UButton>(GetWidgetFromName(TEXT("Button_Game")));
	Btn_Game->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Game);

	Btn_Training = Cast<UButton>(GetWidgetFromName(TEXT("Button_Training")));
	Btn_Training->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Training);

	Btn_Apply = Cast<UButton>(GetWidgetFromName(TEXT("Button_Apply")));
	Btn_Apply->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Apply);

	Btn_Exit = Cast<UButton>(GetWidgetFromName(TEXT("Button_Exit")));
	Btn_Exit->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Exit);

	// SettingPannel
	Pannel_Setting = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Setting")));

	Btn_WindowScreen = Cast<UButton>(GetWidgetFromName(TEXT("Button_Window")));
	Btn_WindowScreen->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_WidowScreen);

	Btn_FullScreen = Cast<UButton>(GetWidgetFromName(TEXT("Button_FullScreen")));
	Btn_FullScreen->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_FullScreen);

	Slider_Sound = Cast<USlider>(GetWidgetFromName(TEXT("Slider_Sound")));
	Slider_Sound->OnValueChanged.AddDynamic(this, &UUserWidget_TrainingSettingMenu::Change_SoundSlider);
	Text_Sound = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Sound")));

	Slider_Mouse = Cast<USlider>(GetWidgetFromName(TEXT("Slider_Mouse")));
	Slider_Mouse->OnValueChanged.AddDynamic(this, &UUserWidget_TrainingSettingMenu::Change_MouseSlider);
	Text_Mouse = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Mouse")));

	Btn_RedTeam = Cast<UButton>(GetWidgetFromName(TEXT("Button_RedTeam")));
	Btn_RedTeam->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_TeamRed);

	Btn_BlueTeam = Cast<UButton>(GetWidgetFromName(TEXT("Button_BlueTeam")));
	Btn_BlueTeam->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_TeamBlue);

	// GamePaanel
	Pannel_Game = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Game")));

	Pannel_Gun1 = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Gun1")));
	Text_Gun1Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun1Name")));
	Text_Gun1Damage = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun1Damage")));
	Text_Gun1Armo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun1Armo")));
	Text_Gun1Type = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun1Type")));

	Pannel_Gun2 = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Gun2")));
	Text_Gun2Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun2Name")));
	Text_Gun2Damage = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun2Damage")));
	Text_Gun2Armo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun2Armo")));
	Text_Gun2Type = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_Gun2Type")));
		

	Btn_ScoreReset = Cast<UButton>(GetWidgetFromName(TEXT("Button_Reset")));
	Btn_ScoreReset->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Reset);


	// TrainingPannel
	TrainingPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Trainning")));

	Btn_TargetStop = Cast<UButton>(GetWidgetFromName(TEXT("Button_Target_Stop")));
	Btn_TargetStop->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_TargetStop);

	Btn_TargetWalk = Cast<UButton>(GetWidgetFromName(TEXT("Button_Target_Walk")));
	Btn_TargetWalk->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_TargetWalk);

	Btn_TargetRun = Cast<UButton>(GetWidgetFromName(TEXT("Button_Target_Run")));
	Btn_TargetRun->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_TargetRun);

	Btn_TargetRandom = Cast<UButton>(GetWidgetFromName(TEXT("Button_Target_Random")));
	Btn_TargetRandom->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_TargetRandom);


	Btn_ScoreReset_1 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Reset_1")));
	Btn_ScoreReset_1->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Reset);

	Btn_Training_Easy = Cast<UButton>(GetWidgetFromName(TEXT("Button_Training_Easy")));
	Btn_Training_Easy->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Training_Easy);

	Btn_Training_Nomal = Cast<UButton>(GetWidgetFromName(TEXT("Button_Training_Nomal")));
	Btn_Training_Nomal->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Training_Nomal);

	Btn_Training_Hard = Cast<UButton>(GetWidgetFromName(TEXT("Button_Training_Hard")));
	Btn_Training_Hard->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Training_Hard);

	Btn_Training_Start = Cast<UButton>(GetWidgetFromName(TEXT("Button_Training_Start")));
	Btn_Training_Start->OnClicked.AddDynamic(this, &UUserWidget_TrainingSettingMenu::OnClick_Training_Start);

}

void UUserWidget_TrainingSettingMenu::OnClick_Setting()
{
	if (Pannel_Setting) {
		Pannel_Setting->SetVisibility(ESlateVisibility::Visible);
	}
	if (Pannel_Game) {
		Pannel_Game->SetVisibility(ESlateVisibility::Hidden);
	}
	if (TrainingPanel) {
		TrainingPanel->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUserWidget_TrainingSettingMenu::OnClick_Game()
{
	if (Pannel_Setting) {
		Pannel_Setting->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Pannel_Game) {
		Pannel_Game->SetVisibility(ESlateVisibility::Visible);
	}
	if (TrainingPanel) {
		TrainingPanel->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUserWidget_TrainingSettingMenu::OnClick_Training()
{
	if (Pannel_Setting) {
		Pannel_Setting->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Pannel_Game) {
		Pannel_Game->SetVisibility(ESlateVisibility::Hidden);
	}
	if (TrainingPanel) {
		TrainingPanel->SetVisibility(ESlateVisibility::Visible);
	}

}

void UUserWidget_TrainingSettingMenu::OnClick_Apply()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Setting Apply"));

	TWeakObjectPtr<AThirdPersonCharacter> Player;
	Player = Cast<AThirdPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


	// 사운드 크기
	auto MyGameinst = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameinst) {
		MyGameinst->AudioSound = soundValue / 50.f;
	}
	
	// 마우스 감도
	if (Player.IsValid()) {
		Player->SetAimSensitivity(mouseValue);
	}

	// 팀 선택
	if (Player.IsValid()) {
		if (RedTeam) {
			Player->SetTeam(ETeamEnum::RedTeam);
		}
		else {
			Player->SetTeam(ETeamEnum::BlueTeam);
		}
	}

	// 타겟 속도

}

void UUserWidget_TrainingSettingMenu::OnClick_Exit()
{
	auto plcont = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (plcont) {
		plcont->CloseSetting();
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_WidowScreen()
{
	UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
	MyGameSettings->SetScreenResolution(FIntPoint(1280,720));
	MyGameSettings->ApplyResolutionSettings(false);
	MyGameSettings->SetFullscreenMode(EWindowMode::Windowed);
	MyGameSettings->ApplySettings(true);
}

void UUserWidget_TrainingSettingMenu::OnClick_FullScreen()
{
	UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
	MyGameSettings->SetScreenResolution(FIntPoint(1920, 1080));
	MyGameSettings->ApplyResolutionSettings(false);
	MyGameSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	MyGameSettings->ApplySettings(true);
}

void UUserWidget_TrainingSettingMenu::OnClick_TeamRed()
{
	RedTeam = true;
}

void UUserWidget_TrainingSettingMenu::OnClick_TeamBlue()
{
	RedTeam = false;
}

void UUserWidget_TrainingSettingMenu::OnClick_TargetStop()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTargetMoveSpeed(EMoveSpeed::Stop);
		//LSA->SetTargetStop();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("UI Stop")));
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_TargetWalk()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTargetMoveSpeed(EMoveSpeed::Walk);
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_TargetRun()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTargetMoveSpeed(EMoveSpeed::Run);
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_TargetRandom()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTargetMoveSpeed(EMoveSpeed::Random);
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_Reset()
{
	auto GameInst = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInst) {
		GameInst->Damage = 0;
		GameInst->Kill = 0;
		GameInst->Death = 0;
	}
}

void UUserWidget_TrainingSettingMenu::Change_SoundSlider(float rate)
{
	soundValue = rate;


	Text_Sound->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)rate)));
}

void UUserWidget_TrainingSettingMenu::Change_MouseSlider(float rate)
{
	mouseValue = rate;

	Text_Mouse->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), rate)));
}

void UUserWidget_TrainingSettingMenu::OnClick_Training_Easy()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTrainingLvl(0);
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_Training_Nomal()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTrainingLvl(1);
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_Training_Hard()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->SetTrainingLvl(2);
	}
}

void UUserWidget_TrainingSettingMenu::OnClick_Training_Start()
{
	auto LSA = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		LSA->StartTraining();
	}
}

void UUserWidget_TrainingSettingMenu::OpenWidget()
{
	if (Pannel_Main) {
		Pannel_Main->SetVisibility(ESlateVisibility::Visible);
	}	

	bOnViewport = true;

	TWeakObjectPtr<AThirdPersonCharacter> Player;
	Player = Cast<AThirdPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// 패널 적용
	OnClick_Setting();

	// 마우스 감도 가져오기
	if (Player.IsValid()) {
		Slider_Mouse->SetValue(Player->GetAimSensitivity());
		Change_MouseSlider(Player->GetAimSensitivity());
	}
	//Text_Mouse->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), mouseValue)));

	// 사운드 가져오기
	auto MyGameinst = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameinst) {
		Slider_Sound->SetValue(MyGameinst->AudioSound * 50.f);
		Change_SoundSlider(MyGameinst->AudioSound * 50.f);
	}
	//Text_Sound->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)soundValue)));

	// 총 적용	
	if (Player.IsValid()) {
		if (Player->GetEquipedGuns()[0].IsValid()) {
			Pannel_Gun1->SetVisibility(ESlateVisibility::Visible);
			Text_Gun1Name->SetText(FText::FromString(Player->GetEquipedGuns()[0]->GunData.GunName));
			Text_Gun1Damage->SetText(FText::FromString(FString::FromInt(Player->GetEquipedGuns()[0]->GunData.Damage)));
			Text_Gun1Armo->SetText(FText::FromString(FString::FromInt((int32)Player->GetEquipedGuns()[0]->GunData.Armo)));
			Text_Gun1Type->SetText(FText::FromString(Player->GetEquipedGuns()[0]->GunData.GunType));
		}
		else {
			Pannel_Gun1->SetVisibility(ESlateVisibility::Hidden);
		}

		if (Player->GetEquipedGuns()[1].IsValid()) {
			Pannel_Gun2->SetVisibility(ESlateVisibility::Visible);
			Text_Gun2Name->SetText(FText::FromString(Player->GetEquipedGuns()[1]->GunData.GunName));
			Text_Gun2Damage->SetText(FText::FromString(FString::FromInt(Player->GetEquipedGuns()[1]->GunData.Damage)));
			Text_Gun2Armo->SetText(FText::FromString(FString::FromInt((int32)Player->GetEquipedGuns()[1]->GunData.Armo)));
			Text_Gun2Type->SetText(FText::FromString(Player->GetEquipedGuns()[1]->GunData.GunType));
		}
		else {
			Pannel_Gun2->SetVisibility(ESlateVisibility::Hidden);
		}
	}


	auto LSA = Cast<ATrainingLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	auto LSA2 = Cast<ALevelActor_TrainningRoom>(GetWorld()->GetLevelScriptActor());
	if (LSA) {
		if (Btn_Game) {
			Btn_Game->SetVisibility(ESlateVisibility::Visible);
		}
		if (Btn_Training) {
			Btn_Training->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (LSA2) {
		if (Btn_Game) {
			Btn_Game->SetVisibility(ESlateVisibility::Hidden);
		}
		if (Btn_Training) {
			Btn_Training->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UUserWidget_TrainingSettingMenu::CloseWidget()
{
	if (Pannel_Main) {
		Pannel_Main->SetVisibility(ESlateVisibility::Hidden);
	}
}
