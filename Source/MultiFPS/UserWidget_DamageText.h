// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "UserWidget_DamageText.generated.h"

/**
 * 
 */
UCLASS()
class MULTIFPS_API UUserWidget_DamageText : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	void SetDamageText(FText _damage, int32 num);

	void StoreWidgetAnimations();

	UWidgetAnimation* GetAnimationByName(FName AnimName) const;

protected:
	TMap<FName, UWidgetAnimation*> AnimationMap;

	class UWidgetAnimation* FloatUpAnimation;

	class UTextBlock* DamageText;
	class UTextBlock* DamageText_Shield;
	class UTextBlock* DamageText_Hp;

	class UTextBlock* DamageText_Crit;
	class UTextBlock* DamageText_Shield_Crit;
	class UTextBlock* DamageText_Hp_Crit;

	TArray<class UTextBlock*> DamageTextArray;

};
