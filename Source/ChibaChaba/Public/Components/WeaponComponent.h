// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHIBACHABA_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

	/**
	 * adds a weapon to the weapon component
	 * @param expected weapon class type to add
	 */
	void AddWeapon(TSubclassOf<ABaseWeapon> WeaponToSpawn);

	/**
	 * @return a copy of the array of weapons added to weapon component
	 */
	const TArray<const ABaseWeapon*> GetAddedWeapons() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABaseWeapon> StartWeapon;

	TArray<TObjectPtr<ABaseWeapon>> AddedWeapons;

	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * Just spawn weapon without attach
	 * @param WeaponToSpawn expected Weapon to Spawn
	 */
	UFUNCTION()
	ABaseWeapon* SpawnWeapon(TSubclassOf<ABaseWeapon> WeaponToSpawn) const;

	/**
	 * Attach Spawned weapon
	 * @param WeaponToAttach expected Weapon to Attach
	 */
	void AttachWeapon(ABaseWeapon* WeaponToAttach);
};
