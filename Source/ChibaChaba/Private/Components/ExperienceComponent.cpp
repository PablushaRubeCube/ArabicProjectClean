// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ExperienceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ChibaChabaGameModeBase.h"

UExperienceComponent::UExperienceComponent()
{
	Experience = 1.f;
	CurrentLevel = 1;
	StartExperienceToLevelUp = 100.f;
	LevelGainRate = 1.1f;
	LevelUpSound = nullptr;
	ExperienceToNextLevel = 100.f;
}

void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner());
}

void UExperienceComponent::AddExperience(float Exp)
{
	if (Exp <= 0) return;
	Experience += Exp;
	while (Experience >= ExperienceToNextLevel)
	{
		Experience -= ExperienceToNextLevel;
		LevelUp();
	}
}

float UExperienceComponent::GetExperience() const
{
	return Experience;
}

float UExperienceComponent::GetExperiencePercent() const
{
	const float PercentExp = (Experience / ExperienceToNextLevel);
	return PercentExp;
}

void UExperienceComponent::LevelUp()
{
	CurrentLevel++;
	ExperienceToNextLevel = CalculeteExperienceToNextLevel(CurrentLevel);

	if (LevelUpSound) UGameplayStatics::PlaySound2D(GetWorld(), LevelUpSound);

	auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) GameMode->LevelUp();
}

float UExperienceComponent::CalculeteExperienceToNextLevel(int32 Lvl)
{
	return FMath::FloorToInt(StartExperienceToLevelUp * FMath::Pow(LevelGainRate, Lvl - 1));
}