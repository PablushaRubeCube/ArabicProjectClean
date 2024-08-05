// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	StartWeapon = nullptr;
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetWorld());
	check(GetOwner());

	AddWeapon(StartWeapon);
}

ABaseWeapon* UWeaponComponent::SpawnWeapon(TSubclassOf<ABaseWeapon> WeaponToSpawn) const
{
	if (!WeaponToSpawn) return nullptr;
	FActorSpawnParameters ActorSpawnParam;
	ActorSpawnParam.Owner = GetOwner();

	return GetWorld()->SpawnActor<ABaseWeapon>(WeaponToSpawn, GetOwner()->GetActorTransform(), ActorSpawnParam);
}

void UWeaponComponent::AttachWeapon(ABaseWeapon* WeaponToAttach)
{
	if (!WeaponToAttach) return;
	const FName SocketName = WeaponToAttach->GetSocketName();
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	WeaponToAttach->AttachToActor(GetOwner(), AttachmentTransformRules, SocketName);
}

void UWeaponComponent::AddWeapon(TSubclassOf<ABaseWeapon> WeaponToSpawn)
{
	for (auto& AddedWeapon : AddedWeapons)
	{
		if (!AddedWeapon) continue;
		if (AddedWeapon->IsA(WeaponToSpawn))
		{
			AddedWeapon->LevelUpWeapon();
			return;
		}
	}

	auto* SpawnedWeapon = SpawnWeapon(WeaponToSpawn);
	if (!SpawnedWeapon) return;
	AddedWeapons.Add(SpawnedWeapon);
	AttachWeapon(SpawnedWeapon);
}

const TArray<const ABaseWeapon*> UWeaponComponent::GetAddedWeapons() const
{
	TArray<const ABaseWeapon*> ConstAddedWeapons;
	ConstAddedWeapons.Reserve(AddedWeapons.Num());
	for (ABaseWeapon* Weapon : AddedWeapons)
	{
		ConstAddedWeapons.Add(Weapon);
	}
	return ConstAddedWeapons;
}
