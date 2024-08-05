// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChiChaCoreTypes.h"
#include "SpawnComponent.generated.h"

class AEnemyCharacter;
class UBehaviorTree;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHIBACHABA_API USpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpawnComponent();

	/**
	 * starts the bot spawn cycle on a timer
	 */
	void StartSpawnBots();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	int32 NumBots;

	UPROPERTY(EditAnywhere, Category = "AI")
	FVector2D RadiusSpawn;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AEnemyCharacter> EnemyCharacter;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BT;

	UPROPERTY(EditAnywhere, Category = "AI")
	float BotsNumGainRate;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SpawnRate;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerChar;

	FTimerHandle SpawnTimerHandler;

	int32 CountWave;
	/**
	 * spawn a certain number of bots
	 * depending on the countwave
	 */
	void SpawnBots();

	/**
	 * @return final spawn location to bot
	 */
	FVector GetSpawnLocation() const;

	/**
	 * calculates spawn location InNavMesh
	 * @param return spawnlacation
	 * @param check FBox Is Within Nav Mesh Bounds
	 */
	void GetSpawnLocationInNavMesh(FVector& SpawnLocation, const FBox& NavBounds) const;

	/**
	 * @return calculates spawn radius with help RadiusSpawn
	 */
	float GetRandomSpawnRadius() const;

	/**
	 * @return NavBounds from level
	 */
	FBox GetNavMeshBounds() const;

	/**
	 * check Spawnlocation is inside NavBounds
	 * @param awaiting spawnlocation for inspection
	 * @param awaiting NavBounds for inspection
	 */
	bool IsWithinNavMeshBounds(const FVector& SpawnLocation, const FBox& NavBounds) const;

	/**
	 * calculates bots num for next spawn wave
	 * @param expect current wave
	 */
	int32 IncrementNextNumBotsWave(int32 LevelWave);

	/**
	 * stop spawn bots when gameover
	 * @param awaiting State for stop spawn bots
	 */
	UFUNCTION()
	void StopSpawnBots(EGameStates State);
};
