// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_TeamSelecUI.h"
#include "ThirdPersonCharacter.h"
#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>

void UUserWidget_TeamSelecUI::NativeConstruct()
{
	Super::NativeConstruct();

	MainCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MainCanvas")));
	RedTeamButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Red")));
	BlueTeamButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Blue")));

	RedTeamButton->OnClicked.AddDynamic(this, &UUserWidget_TeamSelecUI::SelectRedTeam);
	BlueTeamButton->OnClicked.AddDynamic(this, &UUserWidget_TeamSelecUI::SelectBlueTeam);
}

void UUserWidget_TeamSelecUI::SelectBlueTeam()
{
	TWeakObjectPtr<AThirdPersonCharacter> Player;
	Player = Cast<AThirdPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player.IsValid()) {
		//Player->SetTeam(ETeamEnum::BlueTeam);

		CloseWidget();
	}
}

void UUserWidget_TeamSelecUI::SelectRedTeam()
{
	TWeakObjectPtr<AThirdPersonCharacter> Player;
	Player = Cast<AThirdPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player.IsValid()) {
		//Player->SetTeam(ETeamEnum::RedTeam);

		CloseWidget();
	}
}

void UUserWidget_TeamSelecUI::OpenUserWidget()
{
	MainCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MainCanvas")));
	if (MainCanvas && MainCanvas->GetVisibility() == ESlateVisibility::Hidden) {
		MainCanvas->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUserWidget_TeamSelecUI::CloseWidget()
{
	MainCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MainCanvas")));
	if (MainCanvas && MainCanvas->GetVisibility() == ESlateVisibility::Visible) {
		MainCanvas->SetVisibility(ESlateVisibility::Hidden);

		auto plControll = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (plControll) {
			plControll->CloseTeamUI();
		}
	}

}

