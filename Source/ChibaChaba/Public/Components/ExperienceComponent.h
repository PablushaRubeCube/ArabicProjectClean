// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

class USoundBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHIBACHABA_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExperienceComponent();
	/**
	 * adds the desired amount of experience to the character
	 * @param expected amount of experience
	 * you can't add a negative value
	 */
	void AddExperience(float Exp);

	/**
	 *@return the character's current experience
	 */
	float GetExperience() const;

	/**
	 *@return experience as a percentage to the next level
	 */
	UFUNCTION(BlueprintPure)
	float GetExperiencePercent() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Experience")
	float Experience;

	UPROPERTY(EditAnywhere, Category = "Experience")
	int32 CurrentLevel;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float StartExperienceToLevelUp;

	UPROPERTY(EditAnywhere, Category = "Experience")
	float LevelGainRate;

	UPROPERTY(EditAnywhere, Category = "Experience")
	TObjectPtr<USoundBase> LevelUpSound;

	virtual void BeginPlay() override;

private:
	float ExperienceToNextLevel;

	/**
	 * Call when character get levelup and reset exp for next level
	 */
	void LevelUp();

	/**
	 * Calculates the amount of experience needed to get the next level
	 * @param expect current level to calucalate exp
	 */
	float CalculeteExperienceToNextLevel(int32 Lvl);
};
