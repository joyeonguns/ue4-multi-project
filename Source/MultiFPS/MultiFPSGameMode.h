// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiFPSGameMode.generated.h"

UCLASS(minimalapi)
class AMultiFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiFPSGameMode();

	void CollectChat(FString chat);

private:
	UPROPERTY()
		TArray<FString> ChatLog;
};



