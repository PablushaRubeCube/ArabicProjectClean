// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Pickup/Item_Pickup.h"
#include "Item_Heal.generated.h"

UCLASS()
class CHIBACHABA_API AItem_Heal : public AItem_Pickup
{
	GENERATED_BODY()

public:
	AItem_Heal();

protected:
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealValue;

	virtual void BeginPlay() override;

	/**
	 * override func call when OverlapSphere was overlaped
	 * call fx and heal logic
	 */
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
