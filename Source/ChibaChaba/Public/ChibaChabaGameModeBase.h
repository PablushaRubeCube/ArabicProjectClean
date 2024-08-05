// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChiChaCoreTypes.h"
#include "ChibaChabaGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeGameState, EGameStates, State);

class ABaseWeapon;
class UAudioComponent;
class USpawnComponent;
class APlayerCharacter;
class USoundBase;

UCLASS()
class CHIBACHABA_API AChibaChabaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChibaChabaGameModeBase();

	UPROPERTY(BlueprintAssignable)
	FOnChangeGameState OnChangeGameState;

	/**
	 * @return random array weapon type from WeaponsToSpawn
	 */
	TArray<TSubclassOf<ABaseWeapon>> GetArrayWeapons() const;

	/**
	 * change state to claerpause
	 */
	virtual bool ClearPause() override;

	/**
	 * change state to GameOver
	 */
	UFUNCTION(BlueprintCallable)
	void GameOver();

	/**
	 * change state to StartGame
	 */
	UFUNCTION(BlueprintCallable)
	void StartGame();

	/**
	 * change state to LevelUp
	 */
	void LevelUp();

	/**
	 * change state to LevelUp
	 */
	UFUNCTION(BlueprintCallable)
	void OpenCredits();

	/**
	 * @return ref to player character from level
	 */
	APlayerCharacter* GetPlayerCharacter() const;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Music")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TObjectPtr<USpawnComponent> SpawnComponent;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	TArray<TSubclassOf<ABaseWeapon>> WeaponsToSpawn;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	int32 NumWeaponArray;

	UPROPERTY(EditAnywhere, Category = "Music")
	TObjectPtr<USoundBase> BackgroundMusic;

	UPROPERTY(EditAnywhere, Category = "Music")
	TObjectPtr<USoundBase> GameOverMusic;

	virtual void StartPlay() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerChar;

	EGameStates GameState;

	/**
	 * @param set current game state
	 */
	void SetGameState(EGameStates State);

	/**
	 * mark the first launch of the game
	 */
	void FirstLaunchGame();

	/**
	 * start play background music
	 */
	void PlayBackgroundMusic();
};
