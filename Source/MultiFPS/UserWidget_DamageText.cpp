// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_DamageText.h"
#include "Blueprint/WidgetTree.h" // WidgetTree 헤더 파일

void UUserWidget_DamageText::NativeConstruct()
{
	Super::NativeConstruct();
	
	StoreWidgetAnimations();

	FloatUpAnimation = GetAnimationByName("FloatUp");
	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage")));
}

void UUserWidget_DamageText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*if (FloatUpAnimation) {
		PlayAnimation(FloatUpAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}*/
}

void UUserWidget_DamageText::SetDamageText(FText _damage, int32 num)
{
	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage")));
	DamageText_Shield = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage_Shield")));
	DamageText_Hp = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage_Hp")));
	
	DamageText_Crit = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage_Critical")));
	DamageText_Shield_Crit = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage_Shield_Critical")));
	DamageText_Hp_Crit = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Damage_Hp_Critical")));

	DamageTextArray.Add(DamageText);
	DamageTextArray.Add(DamageText_Shield);
	DamageTextArray.Add(DamageText_Hp);
	DamageTextArray.Add(DamageText_Crit);
	DamageTextArray.Add(DamageText_Shield_Crit);
	DamageTextArray.Add(DamageText_Hp_Crit);

	if (DamageText && DamageText_Shield && DamageText_Hp) {
		for (class UTextBlock* damagetxt : DamageTextArray) {
			damagetxt->SetVisibility(ESlateVisibility::Hidden);
			damagetxt->SetText(_damage);
		}

		DamageTextArray[num]->SetVisibility(ESlateVisibility::Visible);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("SetDamageText")) );
	}
}

void UUserWidget_DamageText::StoreWidgetAnimations()
{
	AnimationMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	// check each property of this class
	while (Prop)
	{
		// only evaluate object properties, skip rest
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// only get back properties that are of type widget animation
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				// only get back properties that are of type widget animation
				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Obj);
				// if casting worked update map with new animation
				if (WidgetAnimation && WidgetAnimation->MovieScene)
				{
					FName AnimName = WidgetAnimation->MovieScene->GetFName();
					GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Magenta, AnimName.ToString());
					AnimationMap.Add(AnimName, WidgetAnimation);
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}
}

UWidgetAnimation* UUserWidget_DamageText::GetAnimationByName(FName AnimName) const
{
	UWidgetAnimation* const* WidgetAnimation = AnimationMap.Find(AnimName);
	if (WidgetAnimation)
	{
		return *WidgetAnimation;
	}
	return nullptr;
}
