// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_ChatUI.h"
#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerState.h"
#include "MultiFPSGameMode.h"

void UUserWidget_ChatUI::NativeConstruct()
{
	Super::NativeConstruct();

	editText = Cast<UEditableText>(GetWidgetFromName(TEXT("EditText_Chat")));

	EnterButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Enter")));

	ChatingLogs = Cast<UScrollBox>(GetWidgetFromName(TEXT("Chat_Log")));


	editText->OnTextCommitted.AddDynamic(this, &UUserWidget_ChatUI::CommittedEditTexts);
	EnterButton->OnClicked.AddDynamic(this, &UUserWidget_ChatUI::InputEnter);
}

void UUserWidget_ChatUI::InputEnter()
{
	//InputText();
}

void UUserWidget_ChatUI::CommittedEditTexts(const FText& NewText, ETextCommit::Type CommitType)
{
	if (NewText.ToString() == "") {
		return;
	}

	InputText();
}

void UUserWidget_ChatUI::CreateChatItemOnClient(const FString& newchat)
{
	auto chatItem = CreateWidget<UUserWidget_ChatLogs>(GetWorld(), ChatItemClass);
	if (chatItem) {
		//chatItem->AddToViewport();
		chatItem->Init(newchat);
		ChatingLogs->AddChild(chatItem);
	}
}

void UUserWidget_ChatUI::UpdateChat(TArray<FString> Chalog)
{
	ChatingLogs->ClearChildren();

	for (auto chat : Chalog) {
		CreateChatItemOnClient(chat);
	}
}

void UUserWidget_ChatUI::InputText()
{
	auto playerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (playerController) {
		FString chat = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetPlayerState()->GetPlayerName() + " : " + editText->GetText().ToString();
		//editText->GetText
		playerController->SendTextOnServer(chat);
	}

	editText->SetText(FText::FromString(""));


}
