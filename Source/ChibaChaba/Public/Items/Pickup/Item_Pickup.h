// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Item_Pickup.generated.h"

class UCurveFloat;
class APlayerCharacter;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class CHIBACHABA_API AItem_Pickup : public AItem
{
	GENERATED_BODY()

public:
	AItem_Pickup();

protected:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = "MoveItem")
	float TimerRate;

	UPROPERTY(EditAnywhere, Category = "MoveItem")
	TObjectPtr<UCurveFloat> MoveItemCurve;

	UPROPERTY(EditAnywhere, Category = "MoveItem")
	float TotalTimeMove;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerChar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Overlap")
	TObjectPtr<USphereComponent> OverlapSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (BlueprintProtected))
	TObjectPtr<UNiagaraComponent> NiagaraPickup;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	TObjectPtr<UNiagaraSystem> OverlapParticle;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	TObjectPtr<USoundBase> PickupSound;

	virtual void BeginPlay() override;

	/**
	 * virtual func call when OverlapSphere was overlaped
	 * start pickup action
	 */
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/**
	 * virtual func finish item pickup action
	 */
	virtual void FinishAction();

private:
	float TimerStart;

	FVector OverlapItemLocation;

	/**
	 * Start timer move action.
	 * @return a value depending on whether the checks were successful or not when the timer started
	 */
	bool PickupItem();
	/**
	 * Start to target(char)
	 */
	void MoveToTarget();
	/**
	 * get init move location
	 * @return value depending target is valid or not
	 */
	bool GetMoveItemLocations();
	/**
	 * tick function. call after PickupItem(). Update move action to target
	 * @param init location
	 * @param finish location
	 * @param curve is responsible for the trajectory of movement
	 */
	void UpdateMove(const FVector& StartLocation, const FVector& FinishLocation, const UCurveFloat* Curve);
};
