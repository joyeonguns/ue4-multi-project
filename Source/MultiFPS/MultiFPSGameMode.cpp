// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiFPSGameMode.h"
#include "MultiFPSCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMultiFPSGameMode::AMultiFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMultiFPSGameMode::CollectChat(FString chat)
{
	ChatLog.Add(chat);

	TSubclassOf<AMyPlayerController> playerContollerclass;
	playerContollerclass = AMyPlayerController::StaticClass();

	TArray<AActor*> PlayerControlls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), playerContollerclass, PlayerControlls);


	for (AActor* controll : PlayerControlls) {
		auto con = Cast<AMyPlayerController>(controll);
		con->SetChatLogs(ChatLog);
	}

}
