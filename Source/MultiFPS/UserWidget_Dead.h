// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include "UserWidget_Dead.generated.h"

/**
 * 
 */
UCLASS()
class MULTIFPS_API UUserWidget_Dead : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OpenUI();

	UFUNCTION()
		void OnClick_Respawn();

	void CloseUI();


private:

	class UCanvasPanel* Pannel_Main;

	class UTextBlock* TimerText;
	
	class UButton* RespawnBtn;

	float Timer = 5.0f;
};
