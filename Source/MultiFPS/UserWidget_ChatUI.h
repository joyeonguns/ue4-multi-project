// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/EditableText.h>
#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include "UserWidget_ChatLogs.h"
#include "UserWidget_ChatUI.generated.h"

/**
 *
 */
UCLASS()
class MULTIFPS_API UUserWidget_ChatUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void InputEnter();

	UFUNCTION()
		void CommittedEditTexts(const FText& NewText, ETextCommit::Type CommitType);

	//UFUNCTION(client, Reliable)
	void CreateChatItemOnClient(const FString& newchat);

	void UpdateChat(TArray<FString> Chalogs);

	void InputText();


	UPROPERTY()
		class UEditableText* editText;

private:
	UPROPERTY()
		class UButton* EnterButton;

	UPROPERTY()
		class UScrollBox* ChatingLogs;

	UPROPERTY(EditAnywhere, Category = ui)
		TSubclassOf<UUserWidget_ChatLogs> ChatItemClass;
};

