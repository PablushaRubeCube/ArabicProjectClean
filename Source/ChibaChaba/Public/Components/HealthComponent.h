// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class USoundBase;
class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHIBACHABA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	/**
	 *@return health as a percentage
	 */
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	/**
	 *@return max health
	 */
	float GetMaxHealth() const;

	/**
	 * adds the desired amount of health to the character
	 * @param expected amount of HealthPoint
	 * you can't add a negative value
	 */
	void Heal(float HealthPoint);

	/**
	 * used as alpha for lerp to change health bar
	 * @return the time elapsed since any damage was received, in the range from 0 to 1.
	 */
	UFUNCTION(BlueprintPure)
	float GetCountdownDamagePercentTime() const;

	/**
	 * @return true if the player has recently received damage and before the TotalDamageWaitTime expired.
	 */
	UFUNCTION(BlueprintPure)
	bool WasDamageRecently() const { return DamageRecently; };

protected:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Health | FX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> DamageSound;

	UPROPERTY(EditAnywhere, Category = "Health | FX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> DamagaParticle;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float DamageRate;

	virtual void BeginPlay() override;

private:
	float Health;

	FTimerHandle DamageTimer;

	float StartDamageTimer;

	float TotalDamageWaitTime;

	bool DamageRecently;

	/**
	 * Call timer in func CountdownDamageTime when player get damage
	 */
	void StartCoutdownDamageTimer();
	/**
	 * counts down the time from the moment damage is received to TotalDamageWaitTime
	 */
	void CountdownDamageTime();

	/**
	 * call death func in char
	 */
	void TimeToDie();

	/**
	 * apply damage to health
	 * @param expect damage value.
	 */
	void DamageHealth(float Damage);

	UFUNCTION()
	void TakeAnyDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
