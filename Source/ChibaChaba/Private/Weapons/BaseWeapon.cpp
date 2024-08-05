// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/BaseWeapon.h"
#include "BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SocketName = NAME_None;
	IgnoreClass = nullptr;
	FindToDamageClass = nullptr;
	InitSound = nullptr;
	CurrentLevel = 1;
}

float ABaseWeapon::GetDamage() const
{
	return WeaponCurrentCharacteristics.Damage;
}

const FName& ABaseWeapon::GetSocketName() const
{
	return SocketName;
}

const FWeaponsType& ABaseWeapon::GetWeaponType() const
{
	return WeaponType;
}

void ABaseWeapon::LevelUpWeapon()
{
	if (CurrentLevel < WeaponLevelUpgrades.Num())
	{
		CurrentLevel++;
		if (WeaponLevelUpgrades.IsValidIndex(CurrentLevel - 1))
		{
			WeaponCurrentCharacteristics = WeaponLevelUpgrades[CurrentLevel - 1];
			SetupEffects();
		}
	}
}

int32 ABaseWeapon::GetCurrentLevel() const
{
	return CurrentLevel;
}

void ABaseWeapon::OnActorWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor && !GetWorld()) return;
	if (!IgnoreClass || OtherActor->IsA(IgnoreClass)) return;
	if (!GetOwner() || OtherActor == GetOwner()) return;
	UGameplayStatics::ApplyDamage(OtherActor, WeaponCurrentCharacteristics.Damage, nullptr, this, UDamageType::StaticClass());
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner());

	OnActorBeginOverlap.AddDynamic(this, &ABaseWeapon::OnActorWeaponOverlap);

	if (WeaponLevelUpgrades.IsValidIndex(CurrentLevel - 1))
	{
		WeaponCurrentCharacteristics = WeaponLevelUpgrades[CurrentLevel - 1];
		SetupEffects();
	}
}
