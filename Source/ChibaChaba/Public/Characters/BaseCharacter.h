// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UWeaponComponent;

UCLASS()
class CHIBACHABA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	/**
	 * @return the character's attack montage
	 */
	UFUNCTION(BlueprintPure)
	const UAnimMontage* GetMontageAttack() const;

	/**
	 * Ñauses the character to die
	 */
	virtual void Death();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heath")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditAnywhere, Category = "Material")
	float MaterialChangeRate;

	UPROPERTY(EditAnywhere, Category = "Material")
	float TotalTimeMaterialChange;

	UPROPERTY(EditAnywhere, Category = "Health")
	TArray<TObjectPtr<UAnimMontage>> MontagesDeath;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UAnimMontage> MontageAttack;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle MaterialTimer;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DymMaterials;

	float MaterialChangeStartTime;

	bool StartDie;

	/**
	 * Create dymanic material array for Dissolve material when character die
	 */
	void CreateDymMaterialArray();
	/**
	 * Set param for dymanic material array for Dissolve material when character die
	 */
	void SetDymMaterialParam();
	/**
	 * Init Dymamic array with param
	 */
	void CreateDymMaterial();

	/**
	 * tick function for Dissolve material before char die
	 */
	void ChangeDymMaterialApperance();
	/**
	 * Start tick ChangeDymMaterialApperance
	 */
	void DissolveChar();

	/**
	 * Play Death char animation
	 */
	void PlayDeathAnimation() const;
};
