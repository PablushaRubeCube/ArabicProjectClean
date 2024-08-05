// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class AItem_Pickup;

UCLASS()
class CHIBACHABA_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	/**
	 * Ñauses the AI enemy to die
	 */
	virtual void Death() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector2D RandomMaxSpeedRange;

	// todo make limits 1.0f
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<TSubclassOf<AItem_Pickup>, float> ItemToDrops;

	virtual void BeginPlay() override;

private:
	/**
	 * Set Max enemy speed
	 */
	float GetRandomMaxSpeed() const;

	/**
	 * Call drop loot when enemy die
	 */
	void DropLoot() const;

	/**
	 * @return random drop item from TMap ItemToDrops
	 */
	TSubclassOf<AItem_Pickup> GetRandomDroop() const;
};
