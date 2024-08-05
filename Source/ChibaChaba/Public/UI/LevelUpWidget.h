// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChiChaCoreTypes.h"
#include "LevelUpWidget.generated.h"

class UVerticalBox;

UCLASS()
class CHIBACHABA_API ULevelUpWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> BoxContain;

	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> SelectionWidget;

private:
	virtual void NativeOnInitialized() override;

	/**
	 * call UpdateWeaponsStat when get right state
	 * @param expect levelup state
	 */
	UFUNCTION()
	void LevelUp(EGameStates State);

	/**
	 * fills the BoxContain with widgets with different weapons
	 */
	void UpdateWeaponsStat();
};
