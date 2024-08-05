// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChiChaCoreTypes.h"
#include "PlayerHud.generated.h"

class UUserWidget;

UCLASS()
class CHIBACHABA_API APlayerHud : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> StartGameWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerHUD;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> LevelUpWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> GameOverWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> CreditsWidget;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<EGameStates, TObjectPtr<UUserWidget>> Widgets;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;

	/**
	 * changes visibility depending on state
	 * @param expect state for change visibility
	 */
	UFUNCTION()
	void GameStateChanged(EGameStates State);
};
