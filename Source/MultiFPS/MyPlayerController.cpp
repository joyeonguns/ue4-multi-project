// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MultiFPSGameMode.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>

AMyPlayerController::AMyPlayerController()
{
	SetReplicates(true);
}

void AMyPlayerController::BeginPlay()
{
	OpenUI();


}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Enter"), IE_Pressed, this, &AMyPlayerController::InputEnter);
}


void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerController, ChatLogs);
}


void AMyPlayerController::OnRep_UpdatedChatLogs()
{
	if (ChatUIInstance) {
		ChatUIInstance->UpdateChat(ChatLogs);

	}
}


void AMyPlayerController::SendTextOnServer_Implementation(const FString& chat)
{
	auto gamemode = Cast<AMultiFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gamemode) {
		gamemode->CollectChat(chat);
	}
}

void AMyPlayerController::OpenUI_Implementation()
{
	ChatUIInstance = CreateWidget<UUserWidget_ChatUI>(this, ChatUIClass);
	if (ChatUIInstance) {
		ChatUIInstance->AddToViewport();

		//FInputModeUIOnly mode;
		FInputModeGameOnly mode;
		SetInputMode(mode);
		bShowMouseCursor = false;
	}

	CrosshairInstance = CreateWidget<UUserWidget_Crosshair>(this, CrosshairClass);
	if (CrosshairInstance) {
		CrosshairInstance->AddToViewport();
	}

	if (TeamUIInstance == nullptr) {
		TeamUIInstance = CreateWidget<UUserWidget_TeamSelecUI>(this, TeamUIClass);
		if (TeamUIInstance) {
			TeamUIInstance->AddToViewport();

			OpenTeamUI();
		}
	}
}


void AMyPlayerController::InputEnter()
{
	if (ChatUIInstance) {
		if (!ChatUIInstance->editText->HasKeyboardFocus()) {
			ChatUIInstance->editText->SetKeyboardFocus();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("InputEnter"));
		}
	}
}

void AMyPlayerController::SetChatLogs(TArray<FString> newChatLogs)
{
	ChatLogs = newChatLogs;

}

void AMyPlayerController::Falling()
{
	CrosshairInstance->SetOpenAim();
}

void AMyPlayerController::EndFalling()
{
	CrosshairInstance->DefaultAim();
}

void AMyPlayerController::BindStateComponent(UCharacter_State_Component* StateComponent)
{
	if (StateComponent) {
		CurrentStateComponent = StateComponent;

		if (CurrentStateComponent.IsValid() && CrosshairInstance) {
			CurrentStateComponent->OnDelicate_HpApply.AddUObject(this, &AMyPlayerController::SetHpBar);
			CurrentStateComponent->OnDelicate_ShieldApply.AddUObject(this, &AMyPlayerController::SetShieldBar);
		}
	}
}

void AMyPlayerController::SetHpBar(float ratio)
{
	CrosshairInstance->SetHpbarPercent(ratio);
}

void AMyPlayerController::SetShieldBar(float ratio)
{
	CrosshairInstance->SetShieldbarPercent(ratio);

}

void AMyPlayerController::BindSkillComponent(UCharater_SKill_Component* SkillComponent)
{
	if (SkillComponent) {
		CurrentSkillComponent = SkillComponent;
		if (CurrentSkillComponent.IsValid()) {
			CurrentSkillComponent->OnDelicate_UpdateSkill0CoolTime.AddUObject(this, &AMyPlayerController::SetSkill0CoolTime);
			CurrentSkillComponent->OnDelicate_UpdateSkill1CoolTime.AddUObject(this, &AMyPlayerController::SetSkill1CoolTime);
		}
	}
}

void AMyPlayerController::SetSkill0CoolTime(float cur, float Def)
{
	CrosshairInstance->SetSkill0_CoolTime(cur, Def);
}

void AMyPlayerController::SetSkill1CoolTime(float cur, float Def)
{
	CrosshairInstance->SetSkill1_CoolTime(cur, Def);
}

void AMyPlayerController::OpenTeamUI()
{
	if (TeamUIInstance) {
		FInputModeUIOnly mode;
		SetInputMode(mode);
		bShowMouseCursor = true;

		TeamUIInstance->OpenUserWidget();
	}

}

void AMyPlayerController::CloseTeamUI()
{
	if (TeamUIInstance) {
		FInputModeGameOnly mode;
		SetInputMode(mode);
		bShowMouseCursor = false;
	}
}

UUserWidget_Crosshair* AMyPlayerController::GetCrosshairUI()
{
	if (CrosshairInstance) {
		return CrosshairInstance;
	}

	return nullptr;
}