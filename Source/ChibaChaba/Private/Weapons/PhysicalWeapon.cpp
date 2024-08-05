// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/PhysicalWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPhysicWeapon, All, All)

APhysicalWeapon::APhysicalWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(RootComponent);
}

void APhysicalWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	check(WeaponCollision);
}