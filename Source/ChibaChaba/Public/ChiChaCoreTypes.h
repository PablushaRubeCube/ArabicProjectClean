#pragma once

#include "ChiChaCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EGameStates : uint8
{
	EGS_WaitToStart,
	EGS_InProgress,
	EGS_GameOver,
	EGS_LevelUp,
	EGS_PauseMenu,
	EGS_Credits,

	EGS_MAX
};

class UTexture2D;

USTRUCT(BlueprintType)
struct FWeaponsType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UTexture2D> WeaponIcone = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FText NameWeapon;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	// FText LevelWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FText Overview;
};

USTRUCT(BlueprintType)
struct FWeaponCharacteristics
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Damage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Radius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Size = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 Quantity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float ReloadRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float LifeTime = 0.f;
};
