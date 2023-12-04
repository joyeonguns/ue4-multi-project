// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include "LevelActor_TrainningRoom.h"
#include "UserWidget_TrainingSettingMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIFPS_API UUserWidget_TrainingSettingMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClick_Setting();

	UFUNCTION()
		void OnClick_Game();

	UFUNCTION()
		void OnClick_Training();

	UFUNCTION()
		void OnClick_Apply();

	UFUNCTION()
		void OnClick_Exit();

	UFUNCTION()
		void OnClick_WidowScreen();

	UFUNCTION()
		void OnClick_FullScreen();

	UFUNCTION()
		void OnClick_TeamRed();

	UFUNCTION()
		void OnClick_TeamBlue();

	UFUNCTION()
		void OnClick_TargetStop();

	UFUNCTION()
		void OnClick_TargetWalk();

	UFUNCTION()
		void OnClick_TargetRun();

	UFUNCTION()
		void OnClick_TargetRandom();

	UFUNCTION()
		void OnClick_Reset();

	UFUNCTION()
		void Change_SoundSlider(float rate);

	UFUNCTION()
		void Change_MouseSlider(float rate);

	UFUNCTION()
		void OnClick_Training_Easy();

	UFUNCTION()
		void OnClick_Training_Nomal();

	UFUNCTION()
		void OnClick_Training_Hard();

	UFUNCTION()
		void OnClick_Training_Start();
	
	void OpenWidget();

	void CloseWidget();


	bool bOnViewport = false;


	class UCanvasPanel* Pannel_Main;

private:

	// TopButton
	class UButton* Btn_Setting;
	class UButton* Btn_Game;
	class UButton* Btn_Training;
	class UButton* Btn_Apply;
	class UButton* Btn_Exit;

	// SettingPannel
	class UCanvasPanel* Pannel_Setting;
	
	class UButton* Btn_WindowScreen;
	class UButton* Btn_FullScreen;
	
	class USlider* Slider_Sound;
	class UTextBlock* Text_Sound;

	class USlider* Slider_Mouse;
	class UTextBlock* Text_Mouse;

	class UButton* Btn_RedTeam;
	class UButton* Btn_BlueTeam;

	// GamePaanel
	class UCanvasPanel* Pannel_Game;
		
	class UCanvasPanel* Pannel_Gun1;
	class UTextBlock* Text_Gun1Name;
	class UTextBlock* Text_Gun1Damage;
	class UTextBlock* Text_Gun1Armo;
	class UTextBlock* Text_Gun1Type;

	class UCanvasPanel* Pannel_Gun2;
	class UTextBlock* Text_Gun2Name;
	class UTextBlock* Text_Gun2Damage;
	class UTextBlock* Text_Gun2Armo;
	class UTextBlock* Text_Gun2Type;

	class UButton* Btn_ScoreReset;
	

	// Training Pannel
	class UCanvasPanel* TrainingPanel;

	class UButton* Btn_TargetStop;
	class UButton* Btn_TargetWalk;
	class UButton* Btn_TargetRun;
	class UButton* Btn_TargetRandom;

	class UButton* Btn_ScoreReset_1;

	class UButton* Btn_Training_Easy;
	class UButton* Btn_Training_Nomal;
	class UButton* Btn_Training_Hard;

	class UButton* Btn_Training_Start;





	float soundValue;

	float mouseValue;

	bool RedTeam;

};
