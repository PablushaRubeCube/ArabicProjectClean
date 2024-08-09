// Arabic Game. Copyright by Maksim Kremnev. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/Test_Components.h"
#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ChibaChabaGameModeBase.h"
#include "Components/SpawnComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnBots, "_ChibaChaba.Components.SpawnBots",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

bool FSpawnBots::RunTest(const FString& Paramets)
{
	// Open the specified map
	const FString MapName = ("/Game/_ChibaChaba/Levels/FirstLevel.FirstLevel");
	AutomationOpenMap(MapName);

	// Get the game world
	const auto* World = GetTestGameWorld();
	if (!TestNotNull("World not Exists", World)) return false;

	// Get the authorized game mode
	auto* AuthGameMode = World->GetAuthGameMode();
	if (!TestNotNull("AuthGameMode don't exist", AuthGameMode)) return false;

	// Get the current game mode
	auto* GameMode = Cast<AChibaChabaGameModeBase>(AuthGameMode);
	if (!TestNotNull("GameMode not Exists", GameMode)) return false;

	// get spawn component in game mode
	auto* SpawnComp = GameMode->FindComponentByClass<USpawnComponent>();
	if (!TestNotNull("SpawnComp not Exists", SpawnComp)) return false;

	// Check initial number of enemies
	TArray<AActor*> InActorsOfClass;
	UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), InActorsOfClass);
	if (!TestEqual("Don't find any Enemy in begin", InActorsOfClass.Num(), 1)) return false;

	// Start spawning bots
	SpawnComp->StartSpawnBots();

	// Wait for a specified time to allow bots to spawn
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(6.f));

	// Verify the number of enemies after waiting
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
		[=]()
		{
			if (!World->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Warning, TEXT("World context is not valid"));
				return true;
			}
			TArray<AActor*> OutActorsOfClass;
			UGameplayStatics::GetAllActorsOfClass(World, AEnemyCharacter::StaticClass(), OutActorsOfClass);
			TestEqual("Enemy don't die yet", OutActorsOfClass.Num(), 6);
			return true;
		}))

	return true;
}

#endif