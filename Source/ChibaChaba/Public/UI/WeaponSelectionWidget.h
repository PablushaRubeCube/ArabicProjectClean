// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSelectionWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class ABaseWeapon;

UCLASS()
class CHIBACHABA_API UWeaponSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Set Icone weapon in widget
	 * @param expected icone weapon
	 */
	void SetIconeWeapon(UTexture2D* Icone);
	/**
	 * Set Level weapon in widget
	 * @param expected level weapon
	 */
	void SetLevelWeapon(const FText& Level);
	/**
	 * Set Name weapon in widget
	 * @param expected Name weapon
	 */
	void SetNameWeapon(const FText& Name);
	/**
	 * Set Overview weapon in widget
	 * @param expected Overview weapon
	 */
	void SetOverviewWeapon(const FText& Overview);

	/**
	 * Set Type weapon in widget
	 * @param expected Type weapon
	 */
	void SetWeapon(TSubclassOf<ABaseWeapon> ChooseWeapon);

	/**
	 * @return Weapon type from widget
	 */
	TSubclassOf<ABaseWeapon> GetWeapon() const;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconeWeapon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelWeapon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameWeapon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OverviewWeapon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ChooseButton;

private:
	UPROPERTY()
	TSubclassOf<ABaseWeapon> Weapon;

	virtual void NativeOnInitialized() override;

	/**
	 * add weapon from choose weapon and continue game
	 */
	UFUNCTION()
	void ChooseWeapon();
};
