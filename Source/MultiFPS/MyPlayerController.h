// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UserWidget_ChatUI.h"
#include "UserWidget_Crosshair.h"
#include "Character_State_Component.h"
#include "Charater_SKill_Component.h"
#include "UserWidget_TeamSelecUI.h"
#include "MyPlayerController.generated.h"

/**
 *
 */
UCLASS()
class MULTIFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(server, Reliable)
		void SendTextOnServer(const FString& chat);

	UFUNCTION(client, Reliable)
		void OpenUI();

	void InputEnter();

	void SetChatLogs(TArray<FString> newChatLogs);

	void Falling();

	void EndFalling();

	void BindStateComponent(UCharacter_State_Component* StateComponent);

	void SetHpBar(float ratio);

	void SetShieldBar(float ratio);

	void BindSkillComponent(class UCharater_SKill_Component* SkillComponent);

	void SetSkill0CoolTime(float cur, float Def);

	void SetSkill1CoolTime(float cur, float Def);

	void OpenTeamUI();
	void CloseTeamUI();

	class UUserWidget_Crosshair* GetCrosshairUI();


private:
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget_ChatUI> ChatUIClass;
	UPROPERTY(EditAnywhere, Category = UI)
		class UUserWidget_ChatUI* ChatUIInstance;

	UPROPERTY(ReplicatedUsing = OnRep_UpdatedChatLogs, EditAnywhere)
		TArray<FString> ChatLogs;

	UFUNCTION()
		void OnRep_UpdatedChatLogs();

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget_Crosshair> CrosshairClass;
	UPROPERTY(EditAnywhere, Category = UI)
		class UUserWidget_Crosshair* CrosshairInstance;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget_TeamSelecUI> TeamUIClass;
	UPROPERTY(EditAnywhere, Category = UI)
		class UUserWidget_TeamSelecUI* TeamUIInstance;

	TWeakObjectPtr<class UCharacter_State_Component> CurrentStateComponent;
	TWeakObjectPtr<class UCharater_SKill_Component> CurrentSkillComponent;



};

