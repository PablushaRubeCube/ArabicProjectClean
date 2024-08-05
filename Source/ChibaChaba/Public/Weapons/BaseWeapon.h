// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChiChaCoreTypes.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;
class USoundBase;
class UImage;

UCLASS()
class CHIBACHABA_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	/**
	 * @return Weapon Current Damage value
	 */
	float GetDamage() const;

	/**
	 * @return Socket Name for Attach weapon
	 */
	const FName& GetSocketName() const;

	/**
	 * @return WeaponType for widget sets
	 */
	const FWeaponsType& GetWeaponType() const;

	/**
	 * Level up WeaponCurrentCharacteristics
	 */
	void LevelUpWeapon();

	/**
	 * @return current level weapon
	 */
	UFUNCTION(BlueprintPure)
	int32 GetCurrentLevel() const;

	/**
	 * damage char when overlap actor
	 */
	UFUNCTION(BlueprintCallable)
	virtual void OnActorWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AActor> IgnoreClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AActor> FindToDamageClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (BlueprintProtected))
	TObjectPtr<USoundBase> InitSound;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FWeaponsType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (BlueprintProtected))
	FWeaponCharacteristics WeaponCurrentCharacteristics;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (BlueprintProtected))
	TArray<FWeaponCharacteristics> WeaponLevelUpgrades;

	/**
	 * use in bp for update weapon effects
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetupEffects();

	virtual void BeginPlay() override;

private:
	int32 CurrentLevel;
};
