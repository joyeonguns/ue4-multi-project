#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "UserWidget_ChatLogs.generated.h"

/**
 *
 */
UCLASS()
class MULTIFPS_API UUserWidget_ChatLogs : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Init(FString chat);

private:
	class UTextBlock* textblock;



};
