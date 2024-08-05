// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Pickup/Item_Pickup.h"
#include "Item_Experience.generated.h"

class APlayerCharacter;

UCLASS()
class CHIBACHABA_API AItem_Experience : public AItem_Pickup
{
	GENERATED_BODY()

public:
	AItem_Experience();

	/**
	 * @return the amount of experience this item can give
	 */
	float GetExperience() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Experience")
	float Experience;

	/**
	 * override func call when OverlapSphere was overlaped
	 * set PlayerChar for finishAction
	 */
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/**
	 * add experience after finish move
	 * and destroy item
	 */
	virtual void FinishAction() override;
};
