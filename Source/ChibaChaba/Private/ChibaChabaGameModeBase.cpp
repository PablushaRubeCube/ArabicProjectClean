// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChibaChabaGameModeBase.h"
#include "Components/AudioComponent.h"
#include "Components/SpawnComponent.h"
#include "Characters/PlayerCharacter.h"
#include "ChiChaCoreTypes.h"
#include "Weapons/BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "ChibaChabaGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogAGameMode, All, All);

AChibaChabaGameModeBase::AChibaChabaGameModeBase()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	SetRootComponent(AudioComponent);
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetVolumeMultiplier(1.0f);

	SpawnComponent = CreateDefaultSubobject<USpawnComponent>(TEXT("SpawnComponent"));

	NumWeaponArray = 3;

	BackgroundMusic = nullptr;
	GameOverMusic = nullptr;

	PlayerChar = nullptr;

	GameState = EGameStates::EGS_MAX;
}

TArray<TSubclassOf<ABaseWeapon>> AChibaChabaGameModeBase::GetArrayWeapons() const
{
	TArray<TSubclassOf<ABaseWeapon>> ArrayRandomWeapon;
	TArray<TSubclassOf<ABaseWeapon>> TempArrayWeapons = WeaponsToSpawn;
	while (ArrayRandomWeapon.Num() < NumWeaponArray && TempArrayWeapons.Num() > 0)
	{
		const int32 RandIndex = FMath::RandRange(0, TempArrayWeapons.Num() - 1);
		ArrayRandomWeapon.Add(TempArrayWeapons[RandIndex]);
		TempArrayWeapons.RemoveAtSwap(RandIndex);
	}
	return ArrayRandomWeapon;
}

void AChibaChabaGameModeBase::SetGameState(EGameStates State)
{
	if (GameState == State) return;

	GameState = State;
	OnChangeGameState.Broadcast(State);
}

void AChibaChabaGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GetWorld());
	check(AudioComponent);
	check(SpawnComponent);

	PlayerChar = GetPlayerCharacter();

	PlayBackgroundMusic();

	FirstLaunchGame();
}

bool AChibaChabaGameModeBase::ClearPause()
{
	const bool bCanClear = Super::ClearPause();
	if (bCanClear)
	{
		if (!PlayerChar->Controller) return bCanClear;
		SetGameState(EGameStates::EGS_InProgress);
		auto* CharController = Cast<APlayerController>(PlayerChar->Controller);
		if (CharController) CharController->SetShowMouseCursor(false);
	}
	return bCanClear;
}

void AChibaChabaGameModeBase::GameOver()
{
	if (!PlayerChar->Controller) return;
	auto* CharController = Cast<APlayerController>(PlayerChar->Controller);
	if (CharController) CharController->SetShowMouseCursor(true);

	if (GameOverMusic) UGameplayStatics::PlaySound2D(this, GameOverMusic);

	SetGameState(EGameStates::EGS_GameOver);
}

void AChibaChabaGameModeBase::StartGame()
{
	SpawnComponent->StartSpawnBots();
	SetGameState(EGameStates::EGS_InProgress);
}

void AChibaChabaGameModeBase::LevelUp()
{
	if (!PlayerChar || !PlayerChar->Controller) return;

	auto* CharController = Cast<APlayerController>(PlayerChar->Controller);
	if (CharController) CharController->SetShowMouseCursor(true);
	SetPause(CharController);

	SetGameState(EGameStates::EGS_LevelUp);
}

void AChibaChabaGameModeBase::OpenCredits()
{
	SetGameState(EGameStates::EGS_Credits);
}

APlayerCharacter* AChibaChabaGameModeBase::GetPlayerCharacter() const
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		return Cast<APlayerCharacter>(PlayerController->GetCharacter());
	}
	return nullptr;
}

void AChibaChabaGameModeBase::FirstLaunchGame()
{
	if (!GetWorld()) return;
	auto* CurrentGameInstance = GetWorld()->GetGameInstance<UChibaChabaGameInstance>();
	if (CurrentGameInstance && CurrentGameInstance->GetFirstLaunch())
	{
		CurrentGameInstance->SetFirstLaunch(false);
		SetGameState(EGameStates::EGS_WaitToStart);
	}
	else
	{
		SetGameState(EGameStates::EGS_InProgress);
	}
}

void AChibaChabaGameModeBase::PlayBackgroundMusic()
{
	// TODO replace hardcode for track selection
	if (AudioComponent && BackgroundMusic)
	{
		AudioComponent->SetSound(BackgroundMusic);
		AudioComponent->Play();
	}
}
