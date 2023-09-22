
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
//#include "ThirdPersonCharacter.h"
#include "UserWidget_TeamSelecUI.generated.h"

/**
 *
 */
UCLASS()
class MULTIFPS_API UUserWidget_TeamSelecUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void SelectBlueTeam();

	UFUNCTION()
		void SelectRedTeam();

	void OpenUserWidget();

	void CloseWidget();

private:
	class UCanvasPanel* MainCanvas;
	class UButton* BlueTeamButton;
	class UButton* RedTeamButton;

};
