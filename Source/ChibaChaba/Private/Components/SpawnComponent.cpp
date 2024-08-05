// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SpawnComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "ChibaChabaGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpawnComponent, All, All)

USpawnComponent::USpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	NumBots = 10;
	RadiusSpawn = FVector2D(2500.f, 3500.f);
	EnemyCharacter = nullptr;
	BT = nullptr;
	BotsNumGainRate = 1.1f;
	SpawnRate = 10.f;
	CountWave = 0;
}

void USpawnComponent::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner());
	check(GetWorld());

	auto* GameMode = Cast<AChibaChabaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		PlayerChar = GameMode->GetPlayerCharacter();
		GameMode->OnChangeGameState.AddDynamic(this, &USpawnComponent::StopSpawnBots);
	}
}

// TODO make limits num bots
void USpawnComponent::StartSpawnBots()
{
	if (NumBots > 0) GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandler, this, &USpawnComponent::SpawnBots, SpawnRate, true);
}

void USpawnComponent::SpawnBots()
{
	// TODO make pull bots
	++CountWave;
	for (int32 i = 0; i < IncrementNextNumBotsWave(CountWave); i++)
	{
		// TODO check for valid spawn location
		if (!EnemyCharacter || !BT) return;
		UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), EnemyCharacter, BT, GetSpawnLocation());
	}
}

FVector USpawnComponent::GetSpawnLocation() const
{
	FVector SpawnLocation = FVector::ZeroVector;

	if (!PlayerChar) return SpawnLocation;

	// TODO How justified is it to make this line every bot spawn?
	const FBox NavBounds = GetNavMeshBounds();

	GetSpawnLocationInNavMesh(SpawnLocation, NavBounds);

	return SpawnLocation;
}

void USpawnComponent::GetSpawnLocationInNavMesh(FVector& SpawnLocation, const FBox& NavBounds) const
{
	int32 CountLoop = 0;
	const int32 MaxCountLoop = 6;
	do
	{
		FVector RandomVector = FMath::VRand();
		RandomVector.Z = 0.f;
		RandomVector = RandomVector.GetSafeNormal();
		if (!PlayerChar) return;
		SpawnLocation = PlayerChar->GetActorLocation() + RandomVector * GetRandomSpawnRadius();
		++CountLoop;
	} while (!IsWithinNavMeshBounds(SpawnLocation, NavBounds) && CountLoop < MaxCountLoop);
	if (CountLoop >= MaxCountLoop)
	{
		UE_LOG(LogSpawnComponent, Display, TEXT("Could not find a valid spawn location within the NavMesh bounds after %d CountLoop."),
			CountLoop);
	}
}

float USpawnComponent::GetRandomSpawnRadius() const
{
	return FMath::RandRange(RadiusSpawn.X, RadiusSpawn.Y);
}

FBox USpawnComponent::GetNavMeshBounds() const
{
	FBox NavBounds(ForceInit);

	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		const TSet<FNavigationBounds>& NavBoundsArray = NavSys->GetNavigationBounds();
		for (const FNavigationBounds& Bound : NavBoundsArray)
		{
			NavBounds += Bound.AreaBox;
		}
	}
	return NavBounds;
}

bool USpawnComponent::IsWithinNavMeshBounds(const FVector& SpawnLocation, const FBox& NavBounds) const
{
	return NavBounds.IsInside(SpawnLocation);
}

int32 USpawnComponent::IncrementNextNumBotsWave(int32 LevelWave)
{
	return FMath::FloorToInt(NumBots * FMath::Pow(BotsNumGainRate, LevelWave - 1));
}

void USpawnComponent::StopSpawnBots(EGameStates State)
{
	if (State == EGameStates::EGS_GameOver) GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandler);
}
