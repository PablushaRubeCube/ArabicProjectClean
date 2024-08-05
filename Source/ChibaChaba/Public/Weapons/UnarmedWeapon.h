// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "UnarmedWeapon.generated.h"

class USphereComponent;

UCLASS()
class CHIBACHABA_API AUnarmedWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AUnarmedWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> WeaponCollision;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (BlueprintProtected))
	float DamageRate;

	virtual void BeginPlay() override;

	/**
	 * override func that start TimerDamage for delay damage
	 */
	virtual void OnActorWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	/**
	 * damage ovelaped actor
	 */
	void Damage();

private:
	FTimerHandle TimerDamage;
};
