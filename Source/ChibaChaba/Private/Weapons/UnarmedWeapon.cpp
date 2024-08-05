// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/UnarmedWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AUnarmedWeapon::AUnarmedWeapon()
{
	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	SetRootComponent(WeaponCollision);
	DamageRate = 0.4;
}

void AUnarmedWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponCollision);
}

void AUnarmedWeapon::OnActorWeaponOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnActorWeaponOverlap(OverlappedActor, OtherActor);
	GetWorldTimerManager().SetTimer(TimerDamage, this, &AUnarmedWeapon::Damage, DamageRate, true);
}

void AUnarmedWeapon::Damage()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, FindToDamageClass);
	if (OverlappingActors.Num() <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerDamage);
		return;
	}
	for (auto* OverlappingActor : OverlappingActors)
	{
		if (!OverlappingActor || OverlappingActor->IsA(IgnoreClass)) continue;

		UGameplayStatics::ApplyDamage(OverlappingActor, WeaponCurrentCharacteristics.Damage, nullptr, this, UDamageType::StaticClass());
	}
}