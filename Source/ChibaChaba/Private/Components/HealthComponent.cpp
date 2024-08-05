// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealth, All, All);

UHealthComponent::UHealthComponent()
{
	MaxHealth = 100.0f;

	DamageSound = nullptr;
	DamagaParticle = nullptr;

	DamageRate = 0.1f;

	Health = 0.0f;
	StartDamageTimer = 0.0f;
	TotalDamageWaitTime = 3.0f;
	DamageRecently = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner());
	check(GetWorld());

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeAnyDamage);

	Health = MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return (Health / MaxHealth);
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::Heal(float HealthPoint)
{
	((Health + HealthPoint) < MaxHealth) ? Health += HealthPoint : Health = MaxHealth;
}

float UHealthComponent::GetCountdownDamagePercentTime() const
{
	if (!GetWorld()) return float();
	if (!GetWorld()->GetTimerManager().IsTimerActive(DamageTimer)) return float();
	return (GetWorld()->GetTimeSeconds() - StartDamageTimer) / TotalDamageWaitTime;
}

void UHealthComponent::StartCoutdownDamageTimer()
{
	const auto* PlayerChar = Cast<APlayerCharacter>(GetOwner());
	if (PlayerChar)
	{
		GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &UHealthComponent::CountdownDamageTime, DamageRate, true);
		StartDamageTimer = GetWorld()->GetTimeSeconds();
	}
}

void UHealthComponent::CountdownDamageTime()
{
	if (GetWorld()->GetTimeSeconds() - StartDamageTimer < TotalDamageWaitTime)
	{
		DamageRecently = true;
	}
	else
	{
		DamageRecently = false;
		StartDamageTimer = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
	}
}

void UHealthComponent::TimeToDie()
{
	if (GetOwner())
	{
		if (auto* Chararter = Cast<ABaseCharacter>(GetOwner())) Chararter->Death();
	}
}

void UHealthComponent::DamageHealth(float Damage)
{
	if (Health - Damage > 0)
	{
		Health -= Damage;
	}
	else
	{
		Health = 0.f;
		TimeToDie();
	}
}

void UHealthComponent::TakeAnyDamage(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageSound) UGameplayStatics::PlaySound2D(GetWorld(), DamageSound);
	if (DamagaParticle)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamagaParticle, FVector::ZeroVector, FRotator::ZeroRotator);

	StartCoutdownDamageTimer();

	DamageHealth(Damage);
}
