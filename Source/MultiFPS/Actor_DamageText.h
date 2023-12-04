// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include "Actor_DamageText.generated.h"

UCLASS()
class MULTIFPS_API AActor_DamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_DamageText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamageText(int32 _damage, int32 num, bool bCrit);



private:
	UPROPERTY(EditAnywhere, Category = Comp)
	class UWidgetComponent* WidgetComp;

	FText DamageText;
};
