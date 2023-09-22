// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_ChatLogs.h"

void UUserWidget_ChatLogs::NativeConstruct()
{
	Super::NativeConstruct();

	textblock = Cast<UTextBlock>(GetWidgetFromName("ChatMsg"));
}

void UUserWidget_ChatLogs::Init(FString chat)
{
	textblock = Cast<UTextBlock>(GetWidgetFromName("ChatMsg"));
	if (textblock) {
		textblock->SetText(FText::FromString(chat));
	}
}
