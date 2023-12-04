// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_DamageText.h"
#include "UserWidget_DamageText.h"

// Sets default values
AActor_DamageText::AActor_DamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AActor_DamageText::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_DamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_DamageText::SetDamageText(int32 _damage, int32 num, bool bCrit)
{
	DamageText = FText::FromString(FString::FromInt(_damage));

	UUserWidget_DamageText* DamageWidget = Cast<UUserWidget_DamageText>(WidgetComp->GetUserWidgetObject());
	if (DamageWidget) {
		// 畴农府
		if (!bCrit) {
			// 扁夯
			DamageWidget->SetDamageText(DamageText, num);
			DamageWidget->NativeOnInitialized();
		}
		// 农府
		else {
			DamageWidget->SetDamageText(DamageText, num+3);
			DamageWidget->NativeOnInitialized();
		}
	}


}

